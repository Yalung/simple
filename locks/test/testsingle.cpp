#include "../singleton.h"
#include <stdio.h>
#include <string>

using namespace std;
using namespace simple;

int main() {
    //simple test comiple
    printf("%ld\n", Singleton<string>::Instance()->size());
}
