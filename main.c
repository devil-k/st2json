#include <stdio.h>
#include "jansson/jansson.h"

int main() {
    printf("Hello, World2!\n");

    json_t* sJsonRoot;
    sJsonRoot = json_object();

    json_t* sJsonObj;
    sJsonObj = json_integer(10);

    json_object_set_new(sJsonRoot, "test", sJsonObj);

    printf("Json Test[%s]", json_dumps(sJsonRoot, JSON_ENCODE_ANY));

    return 0;
}
