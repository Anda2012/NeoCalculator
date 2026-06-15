#pragma once

#include <lvgl.h>
#include "../utils/MemoryUtils.h"
#include "../input/KeyCodes.h"
#include "../ui/StatusBar.h"
#include "../math/FractalEngine.h"

#ifdef ARDUINO
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#else
#include "../hal/ArduinoCompat.h"
#endif

#include <atomic>

class FractalApp {
public:
    enum class AppState : uint8_t {
        STATE_ATLAS_LAUNCHER = 0,
        STATE_FRACTAL_MODULE
    };

    FractalApp();
    ~FractalApp();

    void begin();
    void end();
    void load();
    void update();
    void handleKey(const KeyEvent& ev);
    void handleInput(const KeyEvent& ev);

    bool isActive() const { return _screen != nullptr; }
    bool consumeExitRequest();

private:
    struct ModuleCardConfig {
        const char* title;
        const char* badge;
        FractalEngine::FractalType type;
        float defaultCenterX;
        float defaultCenterY;
        float defaultZoom;
        int   defaultMaxIter;
        float defaultSliceZ;
        int   defaultPower;
        float defaultJuliaCRe;
        float defaultJuliaCIm;
    };

    static constexpr int SCREEN_W = 320;
    static constexpr int SCREEN_H = 240;
    static constexpr int BAR_H = ui::StatusBar::HEIGHT;
    static constexpr int CANVAS_H = SCREEN_H - BAR_H;
    static constexpr int MODULE_COUNT = 4;
    static constexpr int PREVIEW_W = 64;
    static constexpr int PREVIEW_H = 64;

    enum class TransitionRequest : uint8_t {
        None = 0,
        EnterModule,
        LeaveModule
    };

    static const ModuleCardConfig kModules[MODULE_COUNT];

    lv_obj_t* _screen;
    ui::StatusBar _statusBar;
    AppState _state = AppState::STATE_ATLAS_LAUNCHER;
    bool _exitRequested = false;

    // Atlas launcher UI
    lv_obj_t* _atlasRoot = nullptr;
    lv_obj_t* _atlasCards[MODULE_COUNT] = {nullptr, nullptr, nullptr, nullptr};
    lv_obj_t* _atlasPreviewImages[MODULE_COUNT] = {nullptr, nullptr, nullptr, nullptr};
    lv_image_dsc_t _atlasPreviewDsc[MODULE_COUNT] = {};
    uint16_t _atlasPreviewPixels[MODULE_COUNT][PREVIEW_W * PREVIEW_H] = {};
    bool _atlasPreviewReady = false;
    int _selectedCard = 0;

    // Fractal module UI
    lv_obj_t* _moduleRoot;
    lv_obj_t* _canvasArea;
    lv_obj_t* _fractalImage;
    lv_obj_t* _loadingLabel;

    // Image buffer (PSRAM-backed for ESP32-S3)
    utils::PSRAMBuffer<uint16_t> _buffer;
    lv_image_dsc_t _imgDsc;

    // Viewport parameters
    float _centerX;
    float _centerY;
    float _zoom;
    float _sliceZ;
    int   _maxIter;
    int   _mandelbulbPower;
    float _juliaCRe;
    float _juliaCIm;
    FractalEngine::FractalType _activeType;
    TransitionRequest _pendingTransition = TransitionRequest::None;
    int _pendingModuleIndex = -1;
    bool _transitionBusy = false;

    // Dual-core FreeRTOS rendering variables
    TaskHandle_t _renderTaskHandle = nullptr;
    volatile bool _renderRequested = false;
    volatile bool _renderComplete  = false;
    volatile bool _abortRequested  = false;
    volatile bool _isIdle          = true;
    volatile int  _completedStrips = 0;
    volatile int  _totalStrips     = 0;
    volatile bool _rebaseRequired  = false;
    volatile int  _renderPass      = 0;

    float _prevZoom = 0.0f;
    int   _prevMaxIter = 0;

    std::atomic<bool> _taskShouldExit{false};
    std::atomic<bool> _taskExited{false};

    FractalEngine::ReferenceOrbit* _orbit = nullptr; // Heap-allocated reference orbit (32KB)

    // Atlas lifecycle
    void buildAtlasLauncher();
    void destroyAtlasLauncher();
    void updateAtlasSelection();
    void generateAtlasPreviewsOnce();

    // Module lifecycle
    bool enterFractalModule(int moduleIndex);
    void leaveFractalModule();
    void createModuleUI();
    void destroyModuleUI();
    bool initializeBuffer();
    void applyModuleDefaults(int moduleIndex);
    bool startRenderTask();
    void stopRenderTask();
    void processPendingTransition();
    bool performEnterModuleTransition(int moduleIndex);
    void performLeaveModuleTransition();
    void refreshModuleFrame();

    void renderFractal();
    void shiftBuffer(int dx, int dy);
    void scaleBuffer(float scaleFactor);

    static void renderTaskWrapper(void* param);
};
