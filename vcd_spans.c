#include <stdio.h>
#include <stdlib.h>
#include "vcd_parser.h"
#include <node_api.h>
#include "vcd_types.h"

#define ASSERT(val, expr) \
    if (expr != napi_ok) { \
        napi_throw(env, val); \
    }

void strcopy(const unsigned char* p, const unsigned char* endp, unsigned char* dst) {
  unsigned char* src;
  src = p;
  while (src < (endp - 1)) {
    *dst = *src;
    src++;
    dst++;
  }
  *dst = 0;
}

// FIXME use a better structure to match strings
int stringEq (
  const unsigned char* gold, // search pattern
  const unsigned char* p,
  const unsigned char* endp
) {
  unsigned char* i;
  unsigned char* j;
  i = gold;
  j = p;
  while (1) {
    if (*i == ' ') { // end of search pattern
      return 0;
    }
    while (*i == *j) { // follow matching trail
      i++;
      j++;
    }
    if ((*i == ' ') && (j == (endp - 1))) { // exact match
      return 1;
    }
    while (*i != ' ') { // skip to the end of pattern word
      i++;
    }
    i++;
    j = p; // try another word
  }
}

int commandSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  napi_env env = state->napi_env;

  id_span_cb_t cb = (id_span_cb_t)state->cppCb;
  cb(ID_COMMAND, state, p, endp);

  if( false ) {
    static int maxTrig = 200;
    if( maxTrig > 0 ) {

      printf("command: (%d) ", state->command);
      for(char* pp = p; pp != endp; pp++) {
        printf("%c", *pp);
      }
      printf("\n");
      // printf("my ptr %d:\n", state->dbgvec);
      for(char* pp = p; pp != endp; pp++) {
        printf("%d,", *pp);
      }
      printf("\n");
      maxTrig--;
    }
  }

  if (state->command == 5) { // $upscope
    state->stackPointer -= 1;
    return 0;
  }

  if (state->command == 8) { // $enddefinitions
    napi_value status, undefined, eventName, eventPayload, return_val;
    ASSERT(status, napi_create_string_latin1(env, "simulation", NAPI_AUTO_LENGTH, &status))
    ASSERT(state->info, napi_set_named_property(env, state->info, "status", status))
    ASSERT(undefined, napi_get_undefined(env, &undefined))
    ASSERT(eventName, napi_create_string_latin1(env, "$enddefinitions", NAPI_AUTO_LENGTH, &eventName))
    // ASSERT(eventPayload, napi_create_string_latin1(env, "payload", NAPI_AUTO_LENGTH, &eventPayload))
    napi_value* argv[] = { &eventName }; // , &eventPayload };
    ASSERT(state->lifee, napi_call_function(env, undefined, state->lifee, 1, *argv, &return_val))
    return 0;
  }

  return 0;
}

int scopeIdentifierSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {

  id_span_cb_t cb = (id_span_cb_t)state->cppCb;
  cb(ID_SCOPEID, state, p, endp);

  napi_env env = state->napi_env;
  // *(endp - 1) = 0; // FIXME NULL termination of ASCII string
  strcopy(p, endp, state->tmpStr);
  napi_value name, obj, stack, top;
  ASSERT(name, napi_create_string_latin1(env, (char*)p, (endp - p - 1), &name))
  ASSERT(obj, napi_create_object(env, &obj))
  ASSERT(state->info, napi_get_named_property(env, state->info, "stack", &stack))
  ASSERT(top, napi_get_element(env, stack, state->stackPointer, &top))
  ASSERT(top, napi_set_named_property(env, top, state->tmpStr, obj))
  state->stackPointer += 1;
  ASSERT(top, napi_set_element(env, stack, state->stackPointer, obj))
  return 0;
}

int varSizeSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  id_span_cb_t cb = (id_span_cb_t)state->cppCb;
  cb(ID_VARSIZE, state, p, endp);

  state->size = strtol((const char *)p, (char **)&endp, 10);
  return 0;
}

int varIdSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  id_span_cb_t cb = (id_span_cb_t)state->cppCb;
  cb(ID_VARID, state, p, endp);

  napi_env env = state->napi_env;
  napi_value varId;
  ASSERT(varId, napi_create_string_latin1(env, (char*)p, (endp - p - 1), &varId))
  ASSERT(state->info, napi_set_named_property(env, state->info, "varId", varId))
  return 0;
}

int varNameSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  id_span_cb_t cb = (id_span_cb_t)state->cppCb;
  cb(ID_VARNAME, state, p, endp);

  napi_env env = state->napi_env;
  // *(endp - 1) = 0; // FIXME NULL termination of ASCII string
  strcopy(p, endp, state->tmpStr);
  napi_value stack, top, varId;
  ASSERT(state->info, napi_get_named_property(env, state->info, "stack", &stack))
  ASSERT(top, napi_get_element(env, stack, state->stackPointer, &top))
  ASSERT(state->info, napi_get_named_property(env, state->info, "varId", &varId))
  ASSERT(state->info, napi_set_named_property(env, top, state->tmpStr, varId))
  return 0;
}

int idSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  id_span_cb_t cb = (id_span_cb_t)state->cppCb;
  cb(ID_IDSPAN, state, p, endp);

  napi_env env = state->napi_env;

  if( false ) {
      printf("start with trigger %s:\n", (char*)state->trigger);
      // printf("my ptr %d:\n", state->dbgvec);
      for(char* pp = p; pp != endp; pp++) {
        printf("%d,", *pp);
      }
      printf("\n");
  }

  if (stringEq((state->trigger), p, endp)) {

    if( false ) {
      static int maxTrig = 2;
      if( maxTrig > 0 ) {

        printf("trigger (%d) %s:\n", state->command, (char*)state->trigger);
        // printf("my ptr %d:\n", state->dbgvec);
        for(const char* pp = p; pp != endp; pp++) {
          printf("%d,", *pp);
        }
        printf("\n");
        maxTrig--;
      }
    }

    napi_value undefined, eventName, eventPayload1, eventPayload2, return_val;
    ASSERT(undefined, napi_get_undefined(env, &undefined))
    ASSERT(eventName, napi_create_string_latin1(env, (char*)p, (endp - p - 1), &eventName))
    ASSERT(eventPayload1, napi_create_int32(env, state->time, &eventPayload1))
    ASSERT(eventPayload2, napi_create_int32(env, state->command, &eventPayload2))
    napi_value* argv[] = { &eventName, &eventPayload1, &eventPayload2 };
    ASSERT(state->triee, napi_call_function(env, undefined, state->triee, 3, *argv, &return_val))
  }
  return 0;
}

int vectorSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  id_span_cb_t cb = (id_span_cb_t)state->cppCb;
  cb(ID_VECTORSPAN, state, p, endp);

  return 0;
}

int timeSpan(vcd_parser_t* state, const unsigned char* p, const unsigned char* endp) {
  id_span_cb_t cb = (id_span_cb_t)state->cppCb;
  cb(ID_TIMESPAN, state, p, endp);

  state->time = strtol((const char *)p, (char **)&endp, 10);
  return 0;
}
