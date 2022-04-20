#include <cassert>
#include <iostream>
#include <cmath>
#include "../util/Vector2D.h"

using std::cout, std::endl;

void run_Vector2D_tests() {
    
    // Creating Vector2D
    Vector2D v1 = Vector2D(3, 7);
    Vector2D v2 = Vector2D(1, 4);
    Vector2D v3 = Vector2D();
    cout << "v1: " << v1 << endl;
    cout << "v2: " << v2 << endl;
    cout << "v3: " << v3 << endl;

    assert(v1.x == 3); assert(v1.y == 7);
    assert(v2.x == 1); assert(v2.y == 4);
    assert(v3.x == 0); assert(v3.y == 0);

    // Copy constructor
    Vector2D v4 = Vector2D(v1);
    cout << "v4: " << v4 << endl;
    assert(v1.x == 3); assert(v1.y == 7);
    assert(v4.x == 3); assert(v4.y == 7);

    // Assignment operator
    v3 = v2;
    cout << "v3: " << v3 << endl;
    assert(v2.x == 1); assert(v2.y == 4);
    assert(v3.x == 1); assert(v3.y == 4);

    // Addition
    Vector2D v5 = v1 + v2;
    cout << "v5: " << v5 << endl;
    assert(v1.x == 3); assert(v1.y == 7);
    assert(v2.x == 1); assert(v2.y == 4);
    assert(v5.x == 4); assert(v5.y == 11);

    // Subtraction
    Vector2D v6 = v3 - v5;
    cout << "v6: " << v6 << endl;
    assert(v3.x == 1); assert(v3.y == 4);
    assert(v5.x == 4); assert(v5.y == 11);
    assert(v6.x == -3); assert(v6.y == -7);    

    // Multiplication
    Vector2D v7 = v6 * v5;
    cout << "v7: " << v7 << endl;
    assert(v5.x == 4); assert(v5.y == 11);
    assert(v6.x == -3); assert(v6.y == -7);    
    assert(v7.x == -12); assert(v7.y == -77);    

    // Multiplication with constant
    Vector2D v8 = v7 * -1;
    cout << "v8: " << v8 << endl;
    assert(v7.x == -12); assert(v7.y == -77);    
    assert(v8.x == 12); assert(v8.y == 77);    

    // Division
    Vector2D v9 = v2 / v1;
    cout << "v9: " << v9 << endl;
    assert(v1.x == 3); assert(v1.y == 7);
    assert(v2.x == 1); assert(v2.y == 4);
    assert(v9.x > 0.3330); assert(v9.x < 0.3339);
    assert(v9.y > 0.5710); assert(v9.y < 0.5719);

    // Division with constant
    Vector2D v10 = v1 / 10;
    cout << "v10: " << v10 << endl;
    assert(v1.x == 3); assert(v1.y == 7);
    assert(v10.x > 0.2999); assert(v10.x < 0.3001);
    assert(v10.y > 0.6999); assert(v10.y < 0.7001);

    // Rotation
    Vector2D v11 = Vector2D(10, 0);
    Vector2D v12 = v11.rotate(M_PI / 2);
    Vector2D v13 = v11.rotate(M_PI / 4);
    cout << "v11: " << v11 << ", v12: " << v12 << ", v13: " << v13 << endl;

    Vector2D v14 = v11.rotate(M_PI * 3 / 2).normalize();
    cout << "v14: " << v14 << endl;

    cout << "Tests succeeded" << endl;

}
