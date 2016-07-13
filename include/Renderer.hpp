#ifndef MYUPLAY_MYENGINE_RENDERER
#define MYUPLAY_MYENGINE_RENDERER

#include <memory>
#include <vector>
#include <array>
#include <unordered_map>
#include <tuple>

#include <SDL2/SDL.h>

#include "Color.hpp"
#include "Constants.hpp"
#include "RenderPlugin.hpp"
#include "RenderTarget.hpp"
#include "Light.hpp"
#include "Object3D.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "Shader/Shader.hpp"

namespace MyUPlay {

namespace MyEngine {

template <typename Base, typename T = float>
class Renderer {

protected:

	int maxTextures;

	int windowX = 0,
			windowY = 0;
	int windowWidth,
	windowHeight;

	int opaqueObjectsLastIndex = -1, transparentObjectsLastIndex = -1;

	SDL_Window* window;

	Color clearColorv;
	T clearAlpha;

public:

	virtual ~Renderer(){}

	SortOrder currentSortOrder = FrontToBack;

	//TODO type? currentBlending;
	BlendingEquation currentBlendEquation;
	BlendingSource currentBlendSrc;
	BlendingDestination currentBlendDst;
	BlendingEquation currentBlendEquationAlpha;
	BlendingSource currentBlendSrcAlpha;
	BlendingDestination currentBlendDstAlpha;

	DepthMode currentDepthFunc;
	DepthMode currentDepthWrite;

	unsigned currentLineWidth;



	bool autoClear = true,
			autoClearColor = true,
			autoClearDepth = true,
			autoClearStencil = true;

	bool alpha = false,
			depth = true,
			stencil = true,
			antialias = false,
			premultipliedAlpha = true,
			preserveDrawingBuffer = false;

	T gammaFactor = 2.0;
	bool gammaInput = false,
			gammaOutput = false;

	bool shadowMapEnabled = false;
	ShadowMapType shadowMapType = PCFShadowMap;
	CullConstant shadowMapCullFace = CullFaceFront;
	bool shadowMapDebug = false,
			shadowMapCascade = false;

	unsigned maxMorphTargets = 8,
			maxMorphNormals = 4;

	bool autoScaleCubeMaps = true;

	struct {
		struct {
			unsigned programs = 0,
					geometries = 0,
					textures = 0;
		} memory;
		struct {
			unsigned calls = 0,
					vertices = 0,
					faces = 0,
					points = 0;
		} renderer;
	} info;

	virtual void setScissor(int x, int y, unsigned width, unsigned height) = 0;
	virtual void setScissorTest(bool enable = true) = 0;

	virtual void setClearColor(const Color&, float alpha) = 0;
	virtual Color getClearColor() const = 0;
	virtual float getClearAlpha() const = 0;

	virtual void clear(bool color = true, bool depth = true, bool stencil = true) = 0;

	virtual void clearColor() = 0;
	virtual void clearDepth() = 0;
	virtual void clearStencil() = 0;
	virtual void clearTarget(RenderTarget& target, bool color = true, bool depth = true, bool stencil = true) = 0;

	virtual unsigned getMaxAnisotripy() const = 0;

	std::tuple<unsigned, unsigned> getSize() const {
		int width, height;
		SDL_GetWindowSize(window, &width, &height);
		return std::make_tuple(width, height);
	}
	void setSize(unsigned width, unsigned height){
		SDL_SetWindowSize(window, width, height);
	}
	void setPos(unsigned x, unsigned y){
		SDL_SetWindowPosition(window, x, y);
	}

	virtual void setViewport(int x, int y, unsigned width, unsigned height) = 0;
	virtual std::tuple<int, int, unsigned, unsigned> getViewport() const = 0;
	virtual void setDefaultViewport() = 0;

	virtual void renderBufferImmediate(const Object3D<T>& object, const Shader::Shader<Base>& shader, const Material<T>& material) = 0;
	virtual void renderBufferDirect(const Camera<T>& camera, const std::vector<Light<T> >& lights, const Fog<T>& fog, const Material<T>& material, const Object3D<T>& object, const Object3D<T>& objectGroup) = 0;

	virtual void render(const Scene<T>& scene, const Camera<T>& camera, RenderTarget* renderTarget = NULL, bool forceClear = false) = 0;

	virtual void setFaceCulling(CullConstant cullFace, CullDirection frontFaceDirection) = 0;
	virtual void setTexture(std::shared_ptr<Texture> texture, unsigned slot = 0) = 0;
	virtual void setRenderTarget(RenderTarget& target) = 0;
	virtual RenderTarget& getRenderTarget() = 0;
	virtual void readRenderTargetPixels(RenderTarget& target, int x, int y, unsigned width, unsigned height, void** buffer) = 0; //TODO Find type for buffer

	/**
	 * Use this function to get a list of display modes for all monitors.
	 * The top level vector is the monitors
	 * The second level is a list of display modes for that monitor
	 */
	std::vector<std::vector<SDL_DisplayMode> > getDisplayModes() const {

		unsigned monitors = SDL_GetNumVideoDisplays();

		std::vector<std::vector<SDL_DisplayMode> > monitorList(monitors);

		for (unsigned i = 0; i < monitors; ++i){

			unsigned displayModes = SDL_GetNumDisplayModes(i);

			monitorList[i].reserve(displayModes);

			for (unsigned j = 0; j < displayModes; ++j){
				SDL_GetDisplayMode(i, j, &monitorList[i][j]);
			}
		}
	}

	/**
	 * This mode alters screen resolution and settings for this program.
	 * This is what most people consider real full screen.
	 */
	void setFullScreen(){
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		SDL_GetWindowPosition(window, &windowX, &windowY);
	}

	/**
	 * In this mode no resolution change is made and the window will take the size of the desktop.
	 * This is useful for quickly changing the window to desktop size without a border.
	 */
	void setFakeFullScreen(){
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
		SDL_GetWindowPosition(window, &windowX, &windowY);
	}

	void setWindowed(){
		SDL_SetWindowFullscreen(window, 0);
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);
		SDL_GetWindowPosition(window, &windowX, &windowY);
	}

};

}

}

#endif
