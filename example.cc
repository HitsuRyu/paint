#include <cmath>
#include <iostream>
#include <vector>

#include "window.h"
#include "button.h"
#include "GUI.h"

enum UserEventType
{
    EVENT_CLOSE = 1,
    EVENT_UP,
    EVENT_DOWN,
    EVENT_LEFT,
    EVENT_RIGHT,
    EVENT_SET,
    EVENT_UNSET,
    EVENT_CLEAR,
    EVENT_CHMODE,
};

std::vector<Point>	g_vector;
int					g_mode = 0;
Point				g_coords(0,0);
Point				g_psize(25,25);

// родительское окно
class MainWindow : public Window
{
public:
    MainWindow() {}
    ~MainWindow() {}

	void OnCreate();
	void OnDraw(Context *cr);
    void OnSizeChanged();
    void OnNotify(Window *child, uint32_t type, const Point &position);
    void OnChildDelete(Window *pChild);
    bool OnKeyPress(uint64_t value);
    bool OnTimeout();

private:
    RGB         m_color;
};

void MainWindow::OnDraw(Context *cr)
{
    std::cout << "MainWindow::OnDraw()" << std::endl;
	Point size = GetSize();
	Point center(size.GetX()/2,size.GetY()/2);

    // зальем прямоугольник окна серым цветом
    cr->SetColor(RGB(0.8,0.8,0.8));
    cr->FillRectangle(Point(0,0), size);

    cr->SetColor(RGB(0,0,0));
    cr->SetLineWidth(3);
    cr->Line(Point(0,600), Point(1000, 600));
	if (g_mode == 0)
		cr->Text(" Moving ", "Cantarel", 17, Point(900, 625), 0x01);
	else if (g_mode == 1)
		cr->Text(" Printing ", "Cantarel", 17, Point(900, 625), 0x01);
	else
		cr->Text(" Erasing ", "Cantarel", 17, Point(900, 625), 0x01);
    
    cr->SetColor(RGB(0,0,0));
    for (int i = 0; i < g_vector.size(); i++)
    {
        cr->FillRectangle(g_vector[i], g_psize);
    }

    cr->SetColor(RGB(0,0,1));
    cr->FillRectangle(g_coords, g_psize);
}

void MainWindow::OnCreate()
{
    std::cout << "MainWindow::OnCreate()" << std::endl;

    AddChild(new Button(" UP ",EVENT_UP), Point(450,600), Point(100,50));
    AddChild(new Button(" DOWN ",EVENT_DOWN), Point(450,650), Point(100,50));
    AddChild(new Button(" <- ",EVENT_LEFT), Point(350,650), Point(100,50));
    AddChild(new Button(" -> ",EVENT_RIGHT), Point(550,650), Point(100,50));
    AddChild(new Button(" SET ",EVENT_SET), Point(150,650), Point(50,50));
    AddChild(new Button("UNSET",EVENT_UNSET), Point(200,650), Point(50,50));
    AddChild(new Button(" CLEAR ",EVENT_CLEAR), Point(150,600), Point(100,50));
	AddChild(new Button(" MODE ",EVENT_CHMODE), Point(750,600), Point(100,50));
}

void MainWindow::OnSizeChanged()
{
    std::cout << "MainWindow::OnSizeChanged()" << std::endl;
}

void	SetPoint(void)
{
	int i = 0;
	int found = 0;
	while (i < g_vector.size())
	{
		if (g_vector[i].GetX() == g_coords.GetX() && g_vector[i].GetY()==g_coords.GetY())
		{
			found++;
			break;
		}
		i++;    
	}
	if (!found)
		g_vector.push_back(g_coords);
}

void	UnsetPoint(void)
{
	auto iter = g_vector.cbegin();
	for (int i = 0; i < g_vector.size(); i++)
	{
		if (g_vector[i].GetX() == g_coords.GetX() && g_vector[i].GetY()==g_coords.GetY())
			 g_vector.erase(iter + i);
	}
}

void MainWindow::OnNotify(Window *child, uint32_t type, const Point &position)
{
    std::cout << "MainWindow::OnNotify()" << std::endl;
    if(type == EVENT_CLOSE)
    {
        DeleteMe();
    }
    else if(type == EVENT_UP && g_coords.GetY() - g_psize.GetY() >= 0)
    {
		if (g_mode == 1)
			SetPoint();
		g_coords.SetY(g_coords.GetY() - g_psize.GetY());
		ReDraw();
    }
    else if(type == EVENT_DOWN && g_coords.GetY() + g_psize.GetY() <= (600 - g_psize.GetY()))
    {
		if (g_mode == 1)
			SetPoint();
		if (g_mode == 2)
			UnsetPoint();
        g_coords.SetY(g_coords.GetY() + g_psize.GetY());
        ReDraw();
    }
    else if(type == EVENT_LEFT && g_coords.GetX() - g_psize.GetX() >= 0)
    {
		if (g_mode == 1)
			SetPoint();
		if (g_mode == 2)
			UnsetPoint();
        g_coords.SetX(g_coords.GetX() - g_psize.GetX());
        ReDraw();
    }
    else if(type == EVENT_RIGHT && g_coords.GetX() + g_psize.GetX() <= (1000 - g_psize.GetX()))
    {
		if (g_mode == 1)
			SetPoint();
		if (g_mode == 2)
			UnsetPoint();
        g_coords.SetX(g_coords.GetX() + g_psize.GetX());
        ReDraw();
    }
    else if(type == EVENT_SET)
    {
		SetPoint();
        ReDraw();
    }
    else if(type == EVENT_UNSET)
    {
        UnsetPoint();
        ReDraw();
    }
    else if(type == EVENT_CLEAR)
    {
        g_vector.clear();
        ReDraw();
    }
	else if(type == EVENT_CHMODE)
    {
        g_mode++;
		if (g_mode == 3)
			g_mode = 0;
		ReDraw();
    }
}

void MainWindow::OnChildDelete(Window *pChild)
{
    std::cout << "MainWindow::OnChildDelete()" << std::endl;
}

bool MainWindow::OnKeyPress(uint64_t keyval)
{
    std::cout << "MainWindow::OnKeyPress(" << keyval << ")" << std::endl;

    if (keyval == 'w' || keyval == 65362)
    {
        if(g_coords.GetY() - g_psize.GetY() >= 0)
		{
			if (g_mode == 1)
				SetPoint();
			if (g_mode == 2)
				UnsetPoint();
			g_coords.SetY(g_coords.GetY() - g_psize.GetY());
		}
    }
    if (keyval == 'a' || keyval == 65361)
    {
        if(g_coords.GetX() - g_psize.GetX() >= 0)
		{
			if (g_mode == 1)
				SetPoint();
			if (g_mode == 2)
				UnsetPoint();
			g_coords.SetX(g_coords.GetX() - g_psize.GetX());
		}
    }
    if (keyval == 's' || keyval == 65364)
    {
        if(g_coords.GetY() + g_psize.GetY() <= (600 - g_psize.GetY()))
		{
			if (g_mode == 1)
				SetPoint();
			if (g_mode == 2)
				UnsetPoint();
			g_coords.SetY(g_coords.GetY() + g_psize.GetY());
		}
    }
    if (keyval == 'd' || keyval == 65363)
    {
        if(g_coords.GetX() + g_psize.GetX() <= (1000 - g_psize.GetX()))
		{
			if (g_mode == 1)
				SetPoint();
			if (g_mode == 2)
				UnsetPoint();
			g_coords.SetX(g_coords.GetX() + g_psize.GetX());
		}
    }
    if (keyval == 'f')
    {
        SetPoint();
    }
    if (keyval == 'c')
    {
        g_vector.clear();
    }
    if (keyval == 'g')
    {
        UnsetPoint();
    }
    if (keyval == 'q')
    {
        DeleteMe();
        return true;
    }
	if (keyval == 'e')
	{
		g_mode++;
		if (g_mode == 3)
			g_mode = 0;
	}
    ReDraw();
    return true;
}

bool MainWindow::OnTimeout()
{
    std::cout << "MainWindow::OnTimeout()" << std::endl;
    return(1);
}

// функция main
int main(int argc, char **argv)
{
    MainWindow *pWindow = new MainWindow;

    int res = Run(argc, argv, pWindow, 1000, 700);

    delete pWindow;

    return res;
}
