/**
 * GraphView.h — Grapher MVC: View layer (Kandinsky pixel-level rendering)
 *
 * This module owns the RGB565 PSRAM buffer (_graphBuf) and implements
 * all pixel-pushing operations: grid, axes, function curves, integrals,
 * tangents, and intersection markers.
 *
 * The View receives pre-evaluated float arrays from the Model and rasterizes
 * them directly into _graphBuf. No LVGL dependencies here — pure Kandinsky.
 */
#pragma once

#include <vector>
#include <cstdint>
#include <functional>

namespace grapher {

/**
 * GraphView — Kandinsky pixel-level rendering engine.
 *
 * Owns the target RGB565 buffer and implements all graphics operations
 * for grid, axes, curves, integrals, tangents, and points of interest (POI).
 */
class GraphView {
public:
    /// Default constructor (with dummy buffer; must call setBuffer before use)
    GraphView() : _graphBuf(nullptr), _bufW(0), _bufH(0)
                 , _xMin(-10.0f), _xMax(10.0f), _yMin(-7.0f), _yMax(7.0f) {}

    /// Constructor. Takes ownership of _graphBuf (must be allocated by caller).
    /// Dimensions are in pixels (typically 320×160 for graph area).
    GraphView(uint16_t* graphBuf, int bufW, int bufH);

    ~GraphView() = default;

    /// Set world-coordinate viewport
    void setViewport(float xMin, float xMax, float yMin, float yMax);

    /// Retrieve current viewport
    void getViewport(float& xMin, float& xMax, float& yMin, float& yMax) const {
        xMin = _xMin; xMax = _xMax; yMin = _yMin; yMax = _yMax;
    }

    /// Clear buffer to white and redraw grid + axes
    void redrawGridAndAxes();

    /// Clear buffer to white (does NOT redraw grid/axes)
    void clearBuffer();

    /// Draw a single line segment directly into the buffer
    /// x0, y0, x1, y1 are in world coordinates; this method performs coordinate transformation
    /// and Bresenham rasterization directly to the buffer
    void drawFunctionSegment(float wx0, float wy0, float wx1, float wy1, uint32_t rgbColor);

    /// Rasterize function curve into buffer (sample-based adaptive curve)
    void drawFunctionCurve(const std::vector<float>& xSamples,
                           const std::vector<float>& ySamples,
                           uint32_t rgbColor);

    /// Draw integral shading: vertical lines from y=0 to y=f(x) in range [x0,x1]
    /// Uses stipple pattern (alternating pixels) to simulate transparency
    void drawAreaUnderCurve(const std::vector<float>& xSamples,
                            const std::vector<float>& ySamples,
                            float x0, float x1,
                            uint32_t rgbColor);

    /// Draw tangent line: compute numerical derivative at xTarget,
    /// then draw a line across the entire screen
    void drawTangent(const std::vector<float>& xSamples,
                     const std::vector<float>& ySamples,
                     float xTarget,
                     uint32_t rgbColor);

    /// Draw intersection marker: small circle + label at (xInt, yInt)
    void drawIntersectionMarker(float xInt, float yInt, uint32_t rgbColor);

    /// Utility: convert world coords to screen pixel coords
    int worldToScreenX(float wx) const;
    int worldToScreenY(float wy) const;

    /// Utility: convert screen pixel coords to world coords
    float screenToWorldX(int sx) const;
    float screenToWorldY(int sy) const;

private:
    uint16_t* _graphBuf;    ///< RGB565 buffer (PSRAM)
    int       _bufW, _bufH; ///< Buffer dimensions in pixels
    float     _xMin, _xMax, _yMin, _yMax;  ///< World viewport

    /// utility: RGB888 → RGB565
    static uint16_t rgb888to565(uint32_t rgb);

    /// Bresenham line into buffer with clipping
    void fastDrawLine(int x0, int y0, int x1, int y1, uint16_t color);

    /// Rasterize grid lines and axes
    void drawGrid();
    void drawAxes();
};

} // namespace grapher

