#include "astrojag_internal.h"

void Line(int x1, int y1, int x2, int y2, std::function<bool(Point&)> cb) {
    Point from(x1,y1), to(x2, y2);
    Line(from,to,cb);
}

void Line(Point from, Point to, std::function<bool(Point&)> cb) {
    float frac = 0.0f;
    float accum = 0.0f;
    int xdiff, ydiff, xstep, ystep;
    int i, j, xp, yp;
    Point p;
    xdiff = (to.x - from.x);
    ydiff = (to.y - from.y);
    xstep = (xdiff > 0 ? 1 : (xdiff < 0 ? -1 : 0));
    ystep = (ydiff > 0 ? 1 : (ydiff < 0 ? -1 : 0));
    if (xdiff == 0 && ydiff == 0) {
        cb(from);
        return;
    }
    if (ydiff == 0) {
        // horizontal line
        for (i = 0; i <= abs(xdiff); i++) {
            p.x = from.x + (xstep * i);
            p.y = from.y;
            cb(p);
        }
    } else if (xdiff == 0) {
        // vertical line
        for (i = 0; i <= abs(ydiff); i++) {
            p.x = from.x;
            p.y = from.y + (ystep * i);
            cb(p);
        }        
    } else {
        // diagonal line
        frac = fabs(ydiff) / fabs(xdiff);
        if (frac <= 1.0f) {
            yp = from.y;
            accum = 0.0f;
            for (i = 0; i <= abs(xdiff); i++) {
                p.x = from.x + (xstep * i);
                p.y = yp;
                cb(p);
                accum += frac;
                while (accum > 0.5) {
                    accum -= 1.0; yp += ystep;
                }
            }
        } else {
            frac = fabs(xdiff) / fabs(ydiff);
            xp = from.x;
            accum = 0.0f;
            for (i = 0; i <= abs(ydiff); i++) {
                p.x = xp;
                p.y = from.y + (ystep * i);
                cb(p);
                accum += frac;
                while (accum > 0.5) {
                    accum -= 1.0; xp += xstep;
                }
            }        
        }
    }
}
