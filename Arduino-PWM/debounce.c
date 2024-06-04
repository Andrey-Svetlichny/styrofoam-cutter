#include "debounce.h"

void debounce(struct debounce_t *d, int input, void (*on)(), void (*off)())
{
    const int MAX = 5000;
    const int THRESHOLD = 4000;

    d->val += 2 * input - 1;
    if (d->val > MAX)
        d->val = MAX;
    if (d->val < -MAX)
        d->val = -MAX;

    if (d->val == THRESHOLD && !d->on)
    {
        d->on = 1;
        on();
    }
    if (d->val == -THRESHOLD && d->on)
    {
        d->on = 0;
        off();
    }
}
