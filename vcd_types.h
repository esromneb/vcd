#pragma once

#define ID_COMMAND     (0)
#define ID_SCOPEID     (1)
#define ID_VARSIZE     (2)
#define ID_VARID       (3)
#define ID_VARNAME     (4)
#define ID_IDSPAN      (5)
#define ID_VECTORSPAN  (6)
#define ID_TIMESPAN    (7)

typedef void (*id_span_cb_t)(const uint8_t id, const unsigned char* p, const unsigned char* endp);
