#ifndef COMMON_SHOW_H
#define COMMON_SHOW_H
#include <array>
#include <fstream>
#include "incubus_common.h"
#include "shape.h"
#include "param/physics_param.h"
#include "draw/idrawengine.h"

#define FONT_PIXEL_CHAR_HEIGHT 18
#define FONT_PIXEL_CHAR_WIDTH 9

class CShowViewCoord
{
public:
    CShowViewCoord()
    {
        m_center.Place(0.0, 0.0);
    }

    sCoord2& ViewCoordCenter() {return m_center;}
    Real_t& ViewCoordScale() {return m_scale;}
    sVect2& ViewCoordWindowSize() {return m_window_size;}

    sCoord2 PhysicalToView(const sCoord2 &physical_pos)
    {
        auto x = m_scale*(physical_pos.x - m_center.x);
        auto y = m_scale*(physical_pos.y - m_center.y);
        return {x, y};
    }

    sCoord2 ViewToPhysical(const sCoord2 &view_pos)
    {
        auto x = view_pos.x/m_scale + m_center.x;
        auto y = view_pos.y/m_scale + m_center.y;
        return {x, y};
    }

    sCoord2 WindowToView(const sCoord2 &wnd_pos)
    {
        auto ww2 = m_window_size.vx/2.0;
        auto wh2 = m_window_size.vy/2.0;
        auto x = (wnd_pos.x - ww2)/ww2;
        auto y = -(wnd_pos.y - wh2)/wh2;
        return {x, y};
    }

    sCoord2 WindowToPhysical(const sCoord2 &wnd_pos)
    {
        return ViewToPhysical(WindowToView(wnd_pos));
    }

    bool PhysicalInView(const CShape &shape, const sCoord2 &pos)
    {
        auto p0 = ViewToPhysical({-1.0, -1.0});
        auto p1 = ViewToPhysical({ 1.0,  1.0});
        CFigure rect;
        rect.MakeRectangleAxes(p0, p1);
        bool in_view = shape.Collision(rect, pos, {0.0, 0.0}, nullptr);
        return in_view;
    }

    sCoord2& Center() {return m_center;}
    Real_t& Scale() {return m_scale;}
    sVect2& WindowSize() {return m_window_size;}

private:
    sCoord2     m_center;
    Real_t      m_scale = 0.1;
    sVect2      m_window_size = {640, 640};
};

/**
 * @brief CShowUtil - класс для вывода графических примитивов
 */
class CShowUtil
{
public:
    CShowUtil(IDrawEnginePtr draw)
        : m_draw(draw)
    {
        std::fill(m_char.begin(), m_char.end(), CHAR_GL_NONE);
    }

    void Init()
    {
        MakeTextLists();
        LoadTextPixels();
        MakeShapesLists();
    }

    void Draw(CShape &shp)
    {
        for (const auto &fig : shp.Figures())
            Draw(fig);
    }

    void Draw(const CFigure &fig)
    {
        switch (fig.type)
        {
        case figNone:                                                   break;
        case figPoint:          DrawPoint(fig.center);                  break;
        case figCircle:         DrawCircle(fig.center, fig.radius);     break;
        case figLine:           DrawLine(fig.pnt[0], fig.pnt[1]);       break;
        case figRectangleAxes:  DrawRectAxes(fig.pnt[0], fig.pnt[1]);   break;
        default:                assert(false);                          break;
        }
    }

    void DrawPoint(const sCoord2 &p)
    {
        m_draw->DrawPoints2({{p.x, p.y}});
    }

    void DrawLine(const sCoord2 &p1, const sCoord2 &p2)
    {
        m_draw->DrawLines2({{p1.x, p1.y}, {p1.x, p1.y}});
    }

    void DrawRectAxes(const sCoord2 &p1, const sCoord2 &p2, const bool fill = false)
    {
        const std::vector<std::array<Real_t, 2>> points = {{p1.x, p1.y}, {p1.x, p2.y}, {p2.x, p2.y}, {p2.x, p1.y}};
        if (fill)
            m_draw->DrawPolygon2(points);
        else
            m_draw->DrawLineLoop2(points);
    }

    void DrawDisk(const sCoord2 *center, const Real_t radius)
    {
        m_draw->PushMatrix();
        m_draw->Translate(center->x, center->y, 0);
        m_draw->Scale(radius, radius, 1.0);
        m_draw->CallList(m_disk);
        m_draw->PopMatrix();
    }

    void DrawCircle(const sCoord2 &center, const Real_t radius)
    {
        m_draw->PushMatrix();
        m_draw->Translate(center.x, center.y, 0);
        m_draw->Scale(radius, radius, 1.0);
        m_draw->CallList(m_circle);
        m_draw->PopMatrix();
    }

    enum enDrawTextType
    {
        dttPrimitives,
        dttPixels,
    };

    /**
     * @brief DrawText - вывести текст на экран
     * @param[in] text - выводимый текст
     * @param[in,out] x - координата начала по горизонтали
     * @param[in,out] y - координата начала по вертикали
     * @param[in] txt_type - тип выводимого текста (способ отрисовки, шрифт)
     * @remark Координаты будут изменены так, что бы вывод текста можно было продолжить следующей командой
     */
    void DrawText(const char* text, Real_t &x, Real_t &y, const enDrawTextType txt_type = dttPixels)
    {
        const char* c = text;
        while (*c != 0)
        {
            //DrawChar(*c, x, y, txt_type);
            c++;
        }
    }

    /**
     * @brief DrawChar - вывести символ на экран
     * @param[in] c - выводимый символ
     * @param[in,out] x - координата начала по горизонтали
     * @param[in,out] y - координата начала по вертикали
     * @remark Координаты будут изменены так, что бы вывод текста можно было продолжить следующей командой
     */
    void DrawChar(const char c, Real_t &x, Real_t &y, const enDrawTextType txt_type = dttPixels)
    {
        switch (txt_type)
        {
        case dttPrimitives:
            DrawCharPrimitives(c, x, y);
            break;
        case dttPixels:
            DrawCharPixels(c, x, y);
            break;
        }
    }

    /**
     * @brief DrawChar - вывести символ на экран с помощью примитивов OpenGL
     * @param[in] c - выводимый символ
     * @param[in,out] x - координата начала по горизонтали
     * @param[in,out] y - координата начала по вертикали
     * @remark Координаты будут изменены так, что бы вывод текста можно было продолжить следующей командой
     */
    void DrawCharPrimitives(const char c, Real_t &x, Real_t &y)
    {
        const Real_t sz = 0.02;
        m_draw->PointSize(2.0);
        m_draw->LineWidth(2.0);
        m_draw->EnablePointSmooth();
        m_draw->EnableLineSmooth();
        m_draw->Color3(0.0, 0.0, 0.0);
        m_draw->PushMatrix();
        m_draw->Translate(x, (y - sz*2.0), 0);
        m_draw->Scale(sz, sz, 1.0);
        if (m_char.at(static_cast<long unsigned int>(c)) == CHAR_GL_NONE)
            assert(false);
        m_draw->CallList(m_char.at(static_cast<long unsigned int>(c)));
        m_draw->PopMatrix();
        x += sz*1.5;
    }

    void DrawCharPixels(const char c, Real_t &x, Real_t &y)
    {
        //m_draw->PointSize(2.0);
        //m_draw->LineWidth(2.0);
        //m_draw->EnablePointSmooth();
        //m_draw->EnableLineSmooth();
        //m_draw->PushMatrix();
        auto ch_px = &m_char_px[static_cast<uint_8>(c)];
        //m_draw->RasterPos2(x, y - Real_t(0.06));
        //m_draw->PixelZoom(Real_t(1.0), Real_t(1.0));
        m_draw->DrawPixels(FONT_PIXEL_CHAR_WIDTH, FONT_PIXEL_CHAR_HEIGHT, ch_px);
        //m_draw->PopMatrix();
        x += 0.03;
    }

private:
    void MakeShapesLists()
    {
        /*m_disk = m_draw->GenLists(2);
        m_circle = m_disk + 1;

        m_draw->NewList(m_disk);
        m_draw->BeginPolygon();
        for (uint i = 0; i < 12; ++i)
        {
            double x = (cos(2*M_PI*i/12));
            double y = (sin(2*M_PI*i/12));
            m_draw->Vertex2(x, y);
        }
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_circle);
        m_draw->BeginLineLoop();
        for (uint i = 0; i < 12; ++i)
        {
            double x = (cos(2*M_PI*i/12));
            double y = (sin(2*M_PI*i/12));
            m_draw->Vertex2(x, y);
        }
        m_draw->End();
        m_draw->EndList();*/
    }
    /**
     * @brief MakeTextLists - инициализация символов для вывода текста
     */
    void MakeTextLists()
    {
        /*m_char.at(0) = m_draw->GenLists(CHARS_COUNT);
        for (uint i = 1; i < CHARS_COUNT; ++i)
            m_char.at(i) = m_char.at(i-1) + 1;

        m_draw->NewList(m_char.at('1'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.5, 1.5);
        m_draw->Vertex2(1, 2);
        m_draw->Vertex2(1, 0);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('2'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0, 2);
        m_draw->Vertex2(1, 2);
        m_draw->Vertex2(1, 1);
        m_draw->Vertex2(0, 0);
        m_draw->Vertex2(1, 0);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('3'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0, 2);
        m_draw->Vertex2(1, 2);
        m_draw->Vertex2(0.5, 1);
        m_draw->Vertex2(1, 0);
        m_draw->Vertex2(0, 0);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('4'));
        m_draw->BeginLines();
        m_draw->Vertex2(0, 2);
        m_draw->Vertex2(0, 1);
        m_draw->Vertex2(0, 1);
        m_draw->Vertex2(1, 1);
        m_draw->Vertex2(1, 2);
        m_draw->Vertex2(1, 0);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('5'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(1, 2);
        m_draw->Vertex2(0, 2);
        m_draw->Vertex2(0, 1);
        m_draw->Vertex2(1, 1);
        m_draw->Vertex2(1, 0);
        m_draw->Vertex2(0, 0);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('6'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(1, 2);
        m_draw->Vertex2(0, 2);
        m_draw->Vertex2(0, 0);
        m_draw->Vertex2(1, 0);
        m_draw->Vertex2(1, 1);
        m_draw->Vertex2(0, 1);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('7'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.0, 2.0);
        m_draw->Vertex2(1.0, 2.0);
        m_draw->Vertex2(0.5, 1.0);
        m_draw->Vertex2(0.5, 0.0);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('8'));
        m_draw->BeginLines();
        m_draw->Vertex2(0, 2);
        m_draw->Vertex2(1, 2);
        m_draw->Vertex2(1, 2);
        m_draw->Vertex2(1, 0);
        m_draw->Vertex2(1, 0);
        m_draw->Vertex2(0, 0);
        m_draw->Vertex2(0, 0);
        m_draw->Vertex2(0, 2);
        m_draw->Vertex2(0, 1);
        m_draw->Vertex2(1, 1);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('9'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.0, 0.0);
        m_draw->Vertex2(1.0, 0.0);
        m_draw->Vertex2(1.0, 2.0);
        m_draw->Vertex2(0.0, 2.0);
        m_draw->Vertex2(0.0, 1.0);
        m_draw->Vertex2(1.0, 1.0);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('0'));
        m_draw->BeginLineLoop();
        m_draw->Vertex2(0.0, 2.0);
        m_draw->Vertex2(1.0, 2.0);
        m_draw->Vertex2(1.0, 0.0);
        m_draw->Vertex2(0.0, 0.0);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('.'));
        //m_draw->BeginPoints();
        //m_draw->Vertex2(0.50, 0.00);
        //m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at(' '));

        m_draw->EndList();

        m_draw->NewList(m_char.at(':'));
        //m_draw->BeginPoints();
        //m_draw->Vertex2(0.50, 1.00);
        //m_draw->Vertex2(0.50, 0.00);
        //m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at(';'));
        //m_draw->BeginPoints();
        //m_draw->Vertex2(0.50, 1.00);
        //m_draw->Vertex2(0.50, 0.00);
        //m_draw->End();
        m_draw->BeginLines();
        m_draw->Vertex2(0.50, 0.00);
        m_draw->Vertex2(0.25, -.25);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('_'));
        m_draw->BeginLines();
        m_draw->Vertex2(0.0, 0.0);
        m_draw->Vertex2(1.0, 0.0);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('='));
        m_draw->BeginLines();
        m_draw->Vertex2(0.0, 0.25);
        m_draw->Vertex2(1.0, 0.25);
        m_draw->Vertex2(0.0, 0.75);
        m_draw->Vertex2(1.0, 0.75);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('('));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.50, 2.00);
        m_draw->Vertex2(0.25, 1.75);
        m_draw->Vertex2(0.25, 0.25);
        m_draw->Vertex2(0.50, 0.00);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at(')'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.50, 2.00);
        m_draw->Vertex2(0.75, 1.75);
        m_draw->Vertex2(0.75, 0.25);
        m_draw->Vertex2(0.50, 0.00);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('a'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->Vertex2(1.00, 0.00);
        m_draw->Vertex2(0.00, 0.00);
        m_draw->Vertex2(0.00, 0.50);
        m_draw->Vertex2(1.00, 0.50);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('b'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.00, 1.50);
        m_draw->Vertex2(0.00, 0.00);
        m_draw->Vertex2(1.00, 0.00);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->Vertex2(0.50, 1.00);
        m_draw->Vertex2(0.00, 0.50);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('c'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(1.00, 0.00);
        m_draw->Vertex2(0.00, 0.00);
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('d'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(1.00, 1.50);
        m_draw->Vertex2(1.00, 0.00);
        m_draw->Vertex2(0.00, 0.00);
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('e'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(1.00, 0.00);
        m_draw->Vertex2(0.00, 0.00);
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->Vertex2(1.00, 0.50);
        m_draw->Vertex2(0.00, 0.50);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('f'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.50, 0.00);
        m_draw->Vertex2(0.50, 1.50);
        m_draw->Vertex2(1.00, 1.50);
        m_draw->End();
        m_draw->BeginLines();
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('g'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.00, -.50);
        m_draw->Vertex2(1.00, -.50);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(0.00, 0.00);
        m_draw->Vertex2(0.50, 0.00);
        m_draw->Vertex2(1.00, 0.50);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('j'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.50, 1.00);
        m_draw->Vertex2(0.50, -.50);
        m_draw->Vertex2(0.00, -.50);
        m_draw->End();
        //m_draw->BeginPoints();
        //m_draw->Vertex2(0.5, 1.5);
        //m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('i'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.50, 0.00);
        m_draw->Vertex2(0.50, 1.00);
        m_draw->Vertex2(0.25, 1.00);
        m_draw->End();
        //m_draw->BeginPoints();
        //m_draw->Vertex2(0.5, 1.5);
        //m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('l'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.50, 1.50);
        m_draw->Vertex2(0.50, 0.00);
        m_draw->Vertex2(0.75, 0.00);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('m'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.00, 0.00);
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(0.50, 1.00);
        m_draw->Vertex2(1.00, 0.75);
        m_draw->Vertex2(1.00, 0.00);
        m_draw->End();
        m_draw->BeginLines();
        m_draw->Vertex2(0.50, 1.00);
        m_draw->Vertex2(0.50, 0.00);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('n'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.00, 0.00);
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(0.50, 1.00);
        m_draw->Vertex2(1.00, 0.75);
        m_draw->Vertex2(1.00, 0.00);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('o'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.00, 0.00);
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->Vertex2(1.00, 0.00);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('p'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.00, -0.50);
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->Vertex2(1.00, 0.50);
        m_draw->Vertex2(0.00, 0.50);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('r'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.00, 0.00);
        m_draw->Vertex2(0.00, 0.75);
        m_draw->Vertex2(0.25, 1.00);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->End();
        m_draw->BeginLines();
        m_draw->Vertex2(0.00, 0.75);
        m_draw->Vertex2(0.00, 1.00);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('s'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.00, 0.00);
        m_draw->Vertex2(1.00, 0.00);
        m_draw->Vertex2(1.00, 0.50);
        m_draw->Vertex2(0.00, 0.50);
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('t'));
        m_draw->BeginLines();
        m_draw->Vertex2(0.50, 1.50);
        m_draw->Vertex2(0.50, 0.00);
        m_draw->Vertex2(0.50, 0.00);
        m_draw->Vertex2(1.00, 0.00);
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('u'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(0.00, 0.00);
        m_draw->Vertex2(0.75, 0.00);
        m_draw->Vertex2(1.00, 0.25);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->End();
        m_draw->BeginLines();
        m_draw->Vertex2(1.00, 0.25);
        m_draw->Vertex2(1.00, 0.00);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('y'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(0.00, 0.50);
        m_draw->Vertex2(1.00, 0.50);
        m_draw->End();
        m_draw->BeginLineStrip();
        m_draw->Vertex2(1.00, 1.00);
        m_draw->Vertex2(1.00, -0.50);
        m_draw->Vertex2(0.00, -0.50);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('A'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.00, 0.00);
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(0.50, 2.00);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->Vertex2(1.00, 0.00);
        m_draw->End();
        m_draw->BeginLines();
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('B'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.00, 1.00);
        m_draw->Vertex2(1.00, 1.00);
        m_draw->Vertex2(1.00, 0.00);
        m_draw->Vertex2(0.00, 0.00);
        m_draw->Vertex2(0.00, 2.00);
        m_draw->Vertex2(0.75, 2.00);
        m_draw->Vertex2(0.75, 1.00);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('G'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(1.0, 2.0);
        m_draw->Vertex2(0.5, 2.0);
        m_draw->Vertex2(0.0, 1.5);
        m_draw->Vertex2(0.0, 0.5);
        m_draw->Vertex2(0.5, 0.0);
        m_draw->Vertex2(1.0, 0.0);
        m_draw->Vertex2(1.0, 1.0);
        m_draw->Vertex2(0.5, 1.0);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('F'));
        m_draw->BeginLines();
        m_draw->Vertex2(1.0, 2.0);
        m_draw->Vertex2(0.0, 2.0);
        m_draw->Vertex2(0.0, 2.0);
        m_draw->Vertex2(0.0, 0.0);
        m_draw->Vertex2(0.0, 1.0);
        m_draw->Vertex2(1.0, 1.0);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('H'));
        m_draw->BeginLines();
        m_draw->Vertex2(0.0, 0.0);
        m_draw->Vertex2(0.0, 2.0);
        m_draw->Vertex2(1.0, 0.0);
        m_draw->Vertex2(1.0, 2.0);
        m_draw->Vertex2(0.0, 1.0);
        m_draw->Vertex2(1.0, 1.0);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('P'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(0.0, 0.0);
        m_draw->Vertex2(0.0, 2.0);
        m_draw->Vertex2(1.0, 2.0);
        m_draw->Vertex2(1.0, 1.0);
        m_draw->Vertex2(0.0, 1.0);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('S'));
        m_draw->BeginLineStrip();
        m_draw->Vertex2(1.0, 2.0);
        m_draw->Vertex2(0.5, 2.0);
        m_draw->Vertex2(0.0, 1.5);
        m_draw->Vertex2(1.0, 0.5);
        m_draw->Vertex2(0.5, 0.0);
        m_draw->Vertex2(0.0, 0.0);
        m_draw->End();
        m_draw->EndList();

        m_draw->NewList(m_char.at('T'));
        m_draw->BeginLines();
        m_draw->Vertex2(0.50, 0.00);
        m_draw->Vertex2(0.50, 2.00);
        m_draw->Vertex2(0.00, 2.00);
        m_draw->Vertex2(1.00, 2.00);
        m_draw->End();
        m_draw->EndList();*/
    }

    struct pxRGBA
    {
        uint_8   red = 0;
        uint_8   green = 0;
        uint_8   blue = 0;
        uint_8   alpha = 0xFF;
    };

    struct pxFontSymbol
    {
        std::array<std::array<pxRGBA, FONT_PIXEL_CHAR_WIDTH>, FONT_PIXEL_CHAR_HEIGHT> pixels;
    };

#pragma pack(push, 1)
    struct sBMP_Header
     {
       uint_16  bfType;
       uint_32  bfSize;
       uint_32  bfReserved;
       uint_32  bfOffBits;
     };
#pragma pack(pop)

    void LoadTextPixels()
    {
        std::ifstream file ("resources/fonts/font_pixel_09_18.bmp");
        const int ch_hg = FONT_PIXEL_CHAR_HEIGHT;
        const int ch_wt = FONT_PIXEL_CHAR_WIDTH;
        if (not file.is_open()) return;
        auto image = new std::array<std::array<pxRGBA, ch_hg*16>, ch_wt*16>;
        sBMP_Header bmp_header;
        file.read(reinterpret_cast<char*>(&bmp_header), sizeof(bmp_header));
        file.seekg(bmp_header.bfOffBits);
        uint xi = 0;
        uint yi = 0;
        while (not file.eof())
        {
            char px[3];
            file.read(px, sizeof(px));
            pxRGBA px_rgba;
            uint_8 al = 0;
            if ((px[0] == px[1]) and (px[1] == px[2]))
                al = static_cast<uint_8>(0xFF - px[0]);
            px_rgba.alpha = al;
            image->at(xi).at(ch_hg*16-1-yi) = px_rgba;
            xi++;
            if (xi >= ch_wt*16)
            {
                xi = 0;
                yi++;
            }
            if (yi >= ch_hg*16)
                break;
        }

        uint ch_num = 0;
        for (uint j = 0; j < 16; ++j)
            for (uint i = 0; i < 16; ++i)
            {
                auto &pxFS = m_char_px[ch_num];
                for (uint x = 0; x < ch_wt; ++x)
                    for (uint y = 0; y < ch_hg; ++y)
                    {
                        auto xx = i*ch_wt+x;
                        auto yy = j*ch_hg+y;
                        pxFS.pixels[ch_hg-1-y][x] = image->at(xx).at(yy);
                    }
                ch_num++;
            }

        delete image;
    }

    enum
    {
        CHARS_COUNT = 256,
        CHAR_GL_NONE = 9999,
    };

    IDrawEnginePtr      m_draw;

    std::array<uint, CHARS_COUNT>         m_char;     ///< массив, хранящий номера списков команд OpenGL для вывода символов текста
    std::array<pxFontSymbol, CHARS_COUNT>   m_char_px;  ///< массив, хранящий пиксели для вывода символов текста

    uint  m_disk;
    uint  m_circle;
};

using CShowUtilPtr = std::shared_ptr<CShowUtil>;

#endif // COMMON_SHOW_H
