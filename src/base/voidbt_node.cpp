#include "voidbt_node.h"
#include <stdio.h>

using namespace godot;
using namespace std;

VoidBTNode::VoidBTNode() {
    printf("VoidBTNode constructor");
}

VoidBTNode::~VoidBTNode() {
    printf("VoidBTNode destructor");
}

// VoidBTStatus VoidBTNode::tick(double delta) {
//     return VoidBTStatus::IDLE;
// }