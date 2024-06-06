#pragma once

#define ASSERT(cond) if (!(cond)) { iprintf("Failed condition: %s\nwhile(1);", #cond); while (1); }