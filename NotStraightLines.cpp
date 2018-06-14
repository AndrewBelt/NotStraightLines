#include "rack.hpp"

using namespace rack;

Plugin *plugin;

static const int NUM_LINES = 60;
static const int NUM_POINTS = 64;

struct NotStraightLines : Module {
	enum ParamIds {
		NUM_PARAMS
	};
	enum InputIds {
		X_INPUT,
		Y_INPUT,
		TRIG_INPUT,
		NUM_INPUTS
	};
	enum OutputIds {
		NUM_OUTPUTS
	};
	enum LightIds {
		NUM_LIGHTS
	};

	float x[NUM_LINES * NUM_POINTS] = {};
	float y[NUM_LINES * NUM_POINTS] = {};
	int index = 0;

	NotStraightLines() : Module(NUM_PARAMS, NUM_INPUTS, NUM_OUTPUTS, NUM_LIGHTS) {}

	void onReset() override {
		memset(x, 0, sizeof(x));
		memset(y, 0, sizeof(y));
		index = 0;
	}

	void step() override {
		if (index < NUM_LINES * NUM_POINTS) {
			x[index] = inputs[X_INPUT].value / 5.f;
			y[index] = inputs[Y_INPUT].value / 5.f;
			index++;
		}
		else  if (!inputs[TRIG_INPUT].active || inputs[TRIG_INPUT].value >= 2.f) {
			index = 0;
		}
	}
};


struct ScopeWidget : TransparentWidget {
	NotStraightLines *module;

	ScopeWidget() {
		box.size = mm2px(Vec(75, 75));
	}

	void draw(NVGcontext *vg) override {
		nvgBeginPath(vg);
		for (int j = 0; j < NUM_LINES; j++) {
			for (int i = 0; i < NUM_POINTS; i++) {
				Vec p = Vec(i, j + 0.5f).div(Vec(NUM_POINTS - 1, NUM_LINES));
				int index = j * NUM_POINTS + i;
				p = p.plus(Vec(module->x[index], module->y[index]).div(Vec(NUM_LINES, NUM_LINES)));
				p = p.mult(box.size);
				if (i == 0)
					nvgMoveTo(vg, p.x, p.y);
				else
					nvgLineTo(vg, p.x, p.y);
			}
		}
		nvgStrokeWidth(vg, 1.0);
		nvgStrokeColor(vg, nvgRGBA(100, 100, 100, 0xff));
		nvgStroke(vg);

		Widget::draw(vg);
	}
};


struct NotStraightLinesWidget : ModuleWidget {
	NotStraightLinesWidget(NotStraightLines *module) : ModuleWidget(module) {
		setPanel(SVG::load(assetPlugin(plugin, "NotStraightLines.svg")));

		addInput(Port::create<PJ301MPort>(mm2px(Vec(2.0, 118.28827)), Port::INPUT, module, NotStraightLines::X_INPUT));
		addInput(Port::create<PJ301MPort>(mm2px(Vec(21.3344, 118.28827)), Port::INPUT, module, NotStraightLines::Y_INPUT));
		addInput(Port::create<PJ301MPort>(mm2px(Vec(40.6688, 118.28827)), Port::INPUT, module, NotStraightLines::TRIG_INPUT));

		ScopeWidget *scopeWidget = Widget::create<ScopeWidget>(mm2px(Vec(26.0, 26.75)));
		scopeWidget->module = module;
		addChild(scopeWidget);
	}
};


void init(Plugin *p) {
	plugin = p;
	p->slug = TOSTRING(SLUG);
	p->version = TOSTRING(VERSION);

	p->addModel(Model::create<NotStraightLines, NotStraightLinesWidget>("Andrew Belt", "NotStraightLines", "Not Straight Lines", VISUAL_TAG));
}
