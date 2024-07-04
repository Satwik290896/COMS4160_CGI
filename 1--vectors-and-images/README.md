# Question 0 [0 points]
## Build Olio
In the terminal:
```
cd olio
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

Check the build was successful by running the Olio executable:
```
./src/olio_exe/olio_exe
```
You should see this output in the terminal:
```
Hello, World!
```
Also, run the tests:
```
./src/tests/olio_tests
```
You should see this output:
```
===============================================================================
test cases: 1 | 1 passed
assertions: - none -
```

# Question 1 [20 points]
## Part 1: Implement the Vector3 class
Implement all the functions in `src/core/vector.cc` marked with "START/END OF YOUR CODE".  Each function and its expected behavior is documented in the corresponding header file (`src/core/vector.h`)

Some notes:
* The subscript operator (operator[]) is needed so that we can access each element of the vector using indices 0, 1, 2 (for x, y, z, respectively); e.g.,
  ```
  Vector3 vec(1, 2, 3);
  cout << vec[1] << endl;  //!< print the y coordinate (2)
  ```
* Two versions of the  * operator are needed:
  * The first version (`Vector3 operator*(double scale) const`) allows a vector to be multiplied by a scalar on the right; e.g.,
    ```
    Vector3 v1(1, 2, 3);
    Vector3 v2 = v1 * 3;
    ```
  * The second version (`Vector3 operator*(double scale, const Vector3 &vec)`), which is declared as a friend function, allows the vector to be multiplied by a scalar on the left; e.g.,
    ```
    Vector3 v1(1, 2, 3);
    Vector3 v2 = 3 * v1;
    ```

## Part 2: Test it!
First, make sure the code compiles and that you can run the main Olio executable:
```
make
./src/olio_exe/olio_exe
```
Second, edit `src/tests/tests.cc` and uncomment the "TestVector3" test case." Then, Compile the code and run the unit tests:
```
make
./src/tests/olio_tests
```
Make sure all tests pass. You should see an output like this:
```
===============================================================================
All tests passed (26 assertions in 1 test case)
```

# Question 2 [5 points]
## Part 1
We will be using the Eigen library for all our vector, matrix. and linear algebra needs; enable some Eigen types/aliases by uncommenting the marked code in `src/core/types.h`. Test Eigen by uncommenting the case "TestVec3d" in the unit tests
(`src/tests/tests.cc`).

The first test in the "Constructors" section will probably fail. This is because unlike our `Vector3` implementation, Eigen does not initialize its vectors or matrices with zeros when they are instantiated. Remove the failing test (`Vec3d v1; REQUIRE( v1[0] == 0);`) from the section.

Note the function name differences between our `Vector3` implementation and Eigen's (e.g., `Vector3::Cross()` vs `Vec3d::cross()` or `Vector3::IsApprox()` vs `Vec3d::isApprox()`). The reason for that is because in this course, we will try to follow Google's C++ style guide (GSG): https://google.github.io/styleguide/cppguide.html.
Since we implemented `Vector3::IsApprox()`, we followed GSG's "upper camel case" rule for function names (https://google.github.io/styleguide/cppguide.html#Function_Names). Eigen follows its own style for functions; we will use Eigen's functions as they are made available to us in the API.

Getting started with Eigen:
* https://eigen.tuxfamily.org/dox/GettingStarted.html
* https://eigen.tuxfamily.org/dox/group__QuickRefPage.html

# Question 3 [20 points]
## Part 1: Implement the Circle class
Create a header file (`circle.h`) and a source file (`circle.cc`) in `src/core/`. Add the two file names to `src/core/CMakeLists.txt` to the appropriate HEADERS and SOURCES lists.

Similar to the `Vector3` class implementation, define and implement a `Circle` class and add the following function definitions and implementations to the header and source files:
* Default constructor: `Circle()`
* Parameterized constructor: `Circle(const Vec2d &center, double radius, const Vec3d &color)`
* Function for setting circle's center: `void SetCenter(const Vec2d &center)`
* Function for setting circle's radius: `void SetRadius(double radius)`
* Function for setting circle's color: `void SetColor(const Vec3d &color)`
* Function for getting circle's center: `Vec2d GetCenter() const`
* Function for getting circle's radius: `double GetRadius() const`
* Function for getting circle's color: `Vec3d GetColor() const`
* Function for testing whether a point is inside the circle or not: `bool IsPointInside(const Vec2d &point) const`

Use `Vec2d` to store the circle's center, and `Vec3d` to store its color.

## Part 2 [5 points]

Test your implementation of `Circle` by uncommenting the "TestCircle" test case in `src/tests/tests.cc`; make sure include "core/circle.h" in the beginning of `tests.cc` (after the other includes already in the file).

Add two new assertions to the test case to test your implementation of `Circle::IsPointInside()`.

# Question 4 [Bonus: 30 points]
Image manipulation exercises: finish implementation of the gamma correction code in `src/core/image_utils.cc` for the following functions:
  1. `GammaCorrectImage64FC3()`
  2. `GammaCorrectImage8UC3()`
  3. `GammaCorrectImage()`
  4. Test your implementation by uncommenting and running the "TestGamma" unit tests in `src/tests/tests.cc`
  5. Test by running the executable:
  ```
  ./src/olio_exe/olio_exe --input ../data/images/cats_dark.png --output cats_gamma.png --gamma 1.8
  ```
  Verify your output is the same as the gamma corrected version of the image below:

  Original (`cats_dark.png`)         | Gamma corrected (`gamma=1.8`)
  :---------------------------------:|:-----------------------------:
  ![original](figures/cats_dark.png) | ![gamma corrected (`gamma=1.8`)](figures/cats_gamma_1.8.png)
