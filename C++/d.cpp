#include <iostream>
using namespace std;

class box
{
public:
	int color_index;
	virtual void set_size(int w);

	box(int ci)
	{
		this->color_index = ci;
	}
};

class squre : public box
{
public:
	int width;

	squre(int ci, int w) : box(ci), width(w) {}
    virtual void prt_info()
    {
        cout << color_index << endl;
        cout << width << endl;
    }


};

class rectangle : public squre
{
public:
    int height;

    rectangle(int ci, int w, int h) : squre(ci, w), height(h) {}

    void prt_info() override
    {
        squre::prt_info(); 
        cout << height << endl;
    }
};

int main()
{
	int w, h;

	cin >> w >> h;

	squre a(1, w);
	rectangle b(2, w, h);

	a.prt_info();
	b.prt_info();

	return 0;
}

