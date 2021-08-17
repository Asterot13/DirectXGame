#include "AppWindow.h"



int main()
{
	AppWindow app;

	if (app.Init())
	{
		while (app.isRun())
		{
			app.Broadcast();
		}
	}

	return 0;
}