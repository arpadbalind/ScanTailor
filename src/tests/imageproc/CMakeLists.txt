add_test_target(imageproc-tests imageproc
    TestBinaryImage.cpp TestReduceThreshold.cpp
    TestSlicedHistogram.cpp
    TestConnCompEraser.cpp TestConnCompEraserExt.cpp
    TestGrayscale.cpp
    TestRasterOp.cpp TestShear.cpp
    TestOrthogonalRotation.cpp
    TestSkewFinder.cpp
    TestScale.cpp
    TestTransform.cpp
    TestMorphology.cpp
    TestPolygonRasterizer.cpp
    TestSeedFill.cpp
    TestSEDM.cpp
    TestRastLineFinder.cpp)
