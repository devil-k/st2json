#include <stdio.h>
#include "jansson/jansson.h"
#include "base64.h"

typedef struct qpcCrtColumn
{
    unsigned int mType;
    char mColName[256];
} qpcCrtColumn;

typedef struct qpcCrtColumnList
{
    unsigned int mCount;           /* Number of expressions on the list */
    unsigned int mAlloc;           /* Number of entries allocated below */
    qpcCrtColumn mColData[32];

} qpcCrtColumnList;

json_t* serialize_qpcCrtColumn(void* aValue) {   
    qpcCrtColumn* sValue = (qpcCrtColumn*)aValue;
    json_t* sJsonRoot = json_object();
    
    json_object_set_new(sJsonRoot, "mType", json_integer(sValue->mType));
    json_object_set_new(sJsonRoot, "mColName", json_string(sValue->mColName));

    return sJsonRoot;
}

void deserialize_qpcCrtColumn(json_t* aJsonRoot, void* aValue) {
    qpcCrtColumn* sValue = (qpcCrtColumn*)aValue;

    sValue->mType = json_integer_value(json_object_get(aJsonRoot, "mType"));
    strcpy(sValue->mColName, json_string_value(json_object_get(aJsonRoot, "mColName")));
}

void print_qpcCrtColumn(void* aValue) {
    qpcCrtColumn* sValue = (qpcCrtColumn*)aValue;
    printf("qpcCrtColumn mType[%d] mColName[%s]\n", sValue->mType, sValue->mColName);
}

json_t* serialize_qpcCrtColumnList(void* aValue) {
    qpcCrtColumnList* sValue = (qpcCrtColumnList*)aValue;
    json_t* sJsonRoot = json_object();

    json_object_set_new(sJsonRoot, "mCount", json_integer(sValue->mCount));
    json_object_set_new(sJsonRoot, "mAlloc", json_integer(sValue->mAlloc));

    json_t* sJsonArray = json_array();
    for (int i = 0; i < sValue->mCount; i++) {
        json_array_append_new(sJsonArray, serialize_qpcCrtColumn(&sValue->mColData[i]));
    }
    json_object_set_new(sJsonRoot, "mColData", sJsonArray);

    return sJsonRoot;
}

void deserialize_qpcCrtColumnList(json_t* aJsonRoot, void* aValue) {
    qpcCrtColumnList* sValue = (qpcCrtColumnList*)aValue;

    sValue->mCount = json_integer_value(json_object_get(aJsonRoot, "mCount"));
    sValue->mAlloc = json_integer_value(json_object_get(aJsonRoot, "mAlloc"));

    json_t* sJsonArrayObj = json_object_get(aJsonRoot, "mColData");
    for (int i = 0; i < sValue->mCount; i++) {
        deserialize_qpcCrtColumn(json_array_get(sJsonArrayObj, i), &sValue->mColData[i]);
    }
}

void print_qpcCrtColumnList(void* aValue) {
    qpcCrtColumnList* sValue = (qpcCrtColumnList*)aValue;

    printf("qpcCrtColumnList mCount[%d] mAlloc[%d]\n", sValue->mCount, sValue->mAlloc);
    for (int i = 0; i < sValue->mCount; i++) print_qpcCrtColumn(&sValue->mColData[i]);
}

int main() {
    printf("Hello, World2!\n");

    json_t* sJsonRoot;
    sJsonRoot = json_object();

    json_t* sJsonObj;
    sJsonObj = json_integer(10);

    json_object_set_new(sJsonRoot, "test", sJsonObj);

    printf("Json Test[%s]\n", json_dumps(sJsonRoot, JSON_ENCODE_ANY));

    test_base64_encoding();
    test_base64_decoding();

    printf("\n\n");

    qpcCrtColumnList sQpcCrtColumnList;
    sQpcCrtColumnList.mAlloc = 15;
    sQpcCrtColumnList.mCount = 3;

    sQpcCrtColumnList.mColData[0].mType = 10;
    snprintf(sQpcCrtColumnList.mColData[0].mColName, 256, "col-1");
    sQpcCrtColumnList.mColData[1].mType = 11;
    snprintf(sQpcCrtColumnList.mColData[1].mColName, 256, "col-2");
    sQpcCrtColumnList.mColData[2].mType = 12;
    snprintf(sQpcCrtColumnList.mColData[2].mColName, 256, "col-3");

    json_t* sJsonRoott = serialize_qpcCrtColumnList(&sQpcCrtColumnList);
    json_error_t sJsonError;
    char* sDumpStr = json_dumps(sJsonRoott, JSON_ENCODE_ANY);

    printf("[%s\n", sDumpStr);

    sJsonRoot = json_loads(sDumpStr, JSON_ENCODE_ANY, &sJsonError);
    qpcCrtColumnList sQpcCrtColumnList2;
    deserialize_qpcCrtColumnList(sJsonRoot, &sQpcCrtColumnList2);

    print_qpcCrtColumnList(&sQpcCrtColumnList2);

    return 0;
}
