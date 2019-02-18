
#include "WindowController.h"

u_int64_t RGB::XColour()
{
	return (B<<8*0) | (G<<8*1) | (R<<8*2) | (0xff<<8*3);
}

Window Win;
Display *Disp;

void InitWindow()
{
	Disp = XOpenDisplay(NULL);
	Win = XCreateSimpleWindow(Disp, RootWindow(Disp, 0), 1, 1, 900, 600, 0, BlackPixel(Disp, 0), BlackPixel(Disp, 0));
	XSelectInput(Disp, Win, KeyPressMask | KeyReleaseMask | ClientMessage);
	XMapWindow(Disp, Win);
	XFlush(Disp);
	//Atom closeMessage = XInternAtom(Disp, "WM_DELETE_WINDOW", True);
	//XSetWMProtocols(Disp, Win, &closeMessage, 1);
}

void DestroyWindow()
{
	XDestroyWindow(Disp, Win);
	XCloseDisplay(Disp);
}



bool PendingEvent(XEvent *event)
{
	if (XPending(Disp))
	{
		XNextEvent(Disp, event);
		return true;
	}
	return false;
}

bool was_it_auto_repeat(Display * d, XEvent * event, int current_type, int next_type){
	/*  Holding down a key will cause 'autorepeat' to send fake keyup/keydown events, but we want to ignore these: '*/
	if(event->type == current_type && XEventsQueued(d, QueuedAfterReading))
	{
		XEvent nev;
		XPeekEvent(d, &nev);
		return (nev.type == next_type && nev.xkey.time == event->xkey.time && nev.xkey.keycode == event->xkey.keycode);
	}
	return false;
}

u64 GetKeyPressed(XEvent *event)
{
	return XLookupKeysym(&(event->xkey), 0);
}

u64 GetKeyReleased(XEvent *event)
{
	if(was_it_auto_repeat(Disp, event, KeyRelease, KeyPress))
	{
		XNextEvent(Disp, event); /* Consume the extra event so we can ignore it. */
		return 0;
	}
	else
	{
		return XLookupKeysym(&(event->xkey), 0);
	}
}




void Sync()
{
	XSync(Disp, 0);
}
void ForceClear()
{
	DrawRectangle(0,0,900,600, {0,0,0});
}
void DrawPixel(int x, int y, RGB colour)
{
	XGCValues values;
	values.foreground = colour.XColour();
	unsigned long mask = GCForeground;
	GC gc = XCreateGC(Disp, Win, mask, &values);
	XDrawPoint(Disp, Win, gc, x, y);
	XFreeGC(Disp, gc);
}
void DrawRectangle(int x, int y, int w, int h, RGB colour)
{
	XGCValues values;
	values.foreground = colour.XColour();
	unsigned long mask = GCForeground;
	GC gc = XCreateGC(Disp, Win, mask, &values);
	XFillRectangle(Disp, Win, gc, x, y, w, h);
	XFreeGC(Disp, gc);
}
void OutlineRectangle(int x, int y, int w, int h, RGB colour)
{
	XGCValues values;
	values.foreground = colour.XColour();
	unsigned long mask = GCForeground;
	GC gc = XCreateGC(Disp, Win, mask, &values);
	XDrawRectangle(Disp, Win, gc, x, y, w, h);
	XFreeGC(Disp, gc);
}
void DrawLine(int x1, int y1, int x2, int y2, RGB colour)
{
	XGCValues values;
	values.foreground = colour.XColour();
	unsigned long mask = GCForeground;
	GC gc = XCreateGC(Disp, Win, mask, &values);
	XDrawLine(Disp, Win, gc, x1, y1, x2, y2);
	XFreeGC(Disp, gc);
}
/*void DrawImage(int x, int y, Image *image)
{
	XGCValues values;
	values.foreground = 0xffffff;
	values.background = 0xffffff;
	unsigned long mask = GCForeground | GCBackground;
	GC gc = XCreateGC(Disp, Win, mask, &values);
	XPutImage(Disp, Win, gc, image->Ximage, 0, 0, x, y, image->Width, image->Height);
	XFreeGC(Disp, gc);
}*/
void DrawText(int x, int y, std::string text, RGB colour)
{
	
	//
	XGCValues values;
	values.foreground = colour.XColour();
	//values.font = font->fid;
	unsigned long mask = GCForeground;// | GCFont;
	GC gc = XCreateGC(Disp, Win, mask, &values);
	//XFontStruct *font = XQueryFont(Disp, XGContextFromGC(gc));
	//XTextItem textItem;
	//textItem.chars = (char *)text.c_str();
	//textItem.nchars = text.length();
	//textItem.font = font->fid;
	//XDrawText(Disp, Win, gc, x, y, &textItem, 1);
	//XFreeFontInfo(0, font, 1);
	//XDrawImageString(Disp, Win, gc, x, y, text.c_str(), text.length());
	XDrawString(Disp, Win, gc, x, y, text.c_str(), text.length());
	XFreeGC(Disp, gc);
}