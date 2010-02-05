/************************************************************************/
/*                                                                      */
/*       Copyright 2009 by Michael Hanselmann and Ullrich Koethe        */
/*       Cognitive Systems Group, University of Hamburg, Germany        */
/*                                                                      */
/*    This file is part of the VIGRA computer vision library.           */
/*    The VIGRA Website is                                              */
/*        http://kogs-www.informatik.uni-hamburg.de/~koethe/vigra/      */
/*    Please direct questions, bug reports, and contributions to        */
/*        ullrich.koethe@iwr.uni-heidelberg.de    or                    */
/*        vigra@informatik.uni-hamburg.de                               */
/*                                                                      */
/*    Permission is hereby granted, free of charge, to any person       */
/*    obtaining a copy of this software and associated documentation    */
/*    files (the "Software"), to deal in the Software without           */
/*    restriction, including without limitation the rights to use,      */
/*    copy, modify, merge, publish, distribute, sublicense, and/or      */
/*    sell copies of the Software, and to permit persons to whom the    */
/*    Software is furnished to do so, subject to the following          */
/*    conditions:                                                       */
/*                                                                      */
/*    The above copyright notice and this permission notice shall be    */
/*    included in all copies or substantial portions of the             */
/*    Software.                                                         */
/*                                                                      */
/*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND    */
/*    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES   */
/*    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND          */
/*    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT       */
/*    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,      */
/*    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING      */
/*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR     */
/*    OTHER DEALINGS IN THE SOFTWARE.                                   */
/*                                                                      */
/************************************************************************/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include "vigra/stdimage.hxx"
#include "unittest.hxx"
#include "vigra/hdf5impex.hxx"
#include "vigra/multi_array.hxx"

using namespace vigra;


class HDF5ExportImportTest
{

public:

    HDF5ExportImportTest()
    {}

	void testScalarUnstridedHDF5ExportImport()
	{
		// export and import data from and to scalar unstrided array

		char hdf5File[] = "testfile_scalar_unstrided.hdf5";

		// data 1: 
		MultiArray<3,int> out_data_1(MultiArrayShape<3>::type(2, 3, 4));
        // ...initialize the array to the test data
        for (int i = 0; i < 24; ++i)
            out_data_1.data () [i] = i - 12;
		//std::cout << "Test (0,0), (0,1), (1,0): " << out_data_1(0,0) << " " << out_data_1(0,1) << " " << out_data_1(1,0) << " " << std::endl;

		// export
		// ...data 1: 
		char hdf5group_1[] = "group/subgroup/data_unstrided";
		writeHDF5(hdf5File, hdf5group_1, out_data_1);

		// import
		// data 1: 
		HDF5ImportInfo infoHDF5_1(hdf5File, hdf5group_1);
        MultiArray<3,int> in_data_1(MultiArrayShape<3>::type(infoHDF5_1.shapeOfDimension(0), infoHDF5_1.shapeOfDimension(1), infoHDF5_1.shapeOfDimension(2)));
        readHDF5(infoHDF5_1, in_data_1);

		// compare content
		should (in_data_1 == out_data_1);
	}


	void testScalarStridedHDF5ExportImport()
	{
		// export and import data from and to scalar strided array

		char hdf5File[] = "testfile_scalar_strided.hdf5";

		// data 1: unstrided
		MultiArray<3,int> out_data_1(MultiArrayShape<3>::type(2, 3, 4));
        // ...initialize the array to the test data
        for (int i = 0; i < 24; ++i)
            out_data_1.data () [i] = i - 12;
		// data 2: strided
		MultiArrayView<2,int,StridedArrayTag> out_data_2(out_data_1.bindAt(1, 0));
		
		// export
		// ...data 2: strided
		char hdf5group_2[] = "group/subgroup/data_strided";
		writeHDF5(hdf5File, hdf5group_2, out_data_2);

		// import
		// data 2: strided
		HDF5ImportInfo infoHDF5_2(hdf5File, hdf5group_2);
		// import to strided array
		MultiArray<3,int> in_data_1(MultiArrayShape<3>::type(3, infoHDF5_2.shapeOfDimension(0), infoHDF5_2.shapeOfDimension(1))); // strided
		in_data_1.init(42);
		MultiArrayView<2,int,StridedArrayTag> in_data_2(in_data_1.bindInner(0));
        readHDF5(infoHDF5_2, in_data_2);

		// compare content
		should (in_data_2 == out_data_2);
	}


	void testRGBValueUnstridedHDF5ExportImport()
	{
		// export and import data from and to scalar unstrided array

		char hdf5File[] = "testfile_rgbvalue_unstrided.hdf5";

		// data 1: 3D multi array
		// create RGB image from that
		MultiArray< 2, RGBValue<double> > out_data_1(MultiArrayShape<2>::type(5, 8));
        // ...initialize the array to the test data
        for (int i = 0; i < 40; ++i)
            out_data_1.data () [i] = RGBValue<double>(i + 0.1, i + 0.2, i + 0.3);

		// export
		// ...data 2: RGB image
		char hdf5group_1[] = "group/subgroup/data_rgb";
		writeHDF5(hdf5File, hdf5group_1, out_data_1);

		// import
		// data 1: 
		HDF5ImportInfo infoHDF5_1(hdf5File, hdf5group_1);
        MultiArray< 2, RGBValue<double> > in_data_1(MultiArrayShape<2>::type(infoHDF5_1.shapeOfDimension(1), infoHDF5_1.shapeOfDimension(2)));
        readHDF5(infoHDF5_1, in_data_1);

		// compare content
		should (in_data_1 == out_data_1);
	}


	void testRGBValueStridedHDF5ExportImport()
	{
		// export and import data from and to scalar unstrided array

		char hdf5File[] = "testfile_rgbvalue_strided.hdf5";

		// data 1: 3D multi array
		// create RGB image from that
		MultiArray< 3, RGBValue<double> > out_data_1(MultiArrayShape<3>::type(5, 3, 8));
        // ...initialize the array to the test data
        for (int i = 0; i < 120; ++i)
            out_data_1.data () [i] = RGBValue<double>(i + 0.1, i + 0.2, i + 0.3);
		MultiArrayView< 2, RGBValue<double>, StridedArrayTag> out_data_2(out_data_1.bindAt(1, 0));

		// export
		// ...data 2: RGB image
		char hdf5group_2[] = "group/subgroup/data_rgb";
		writeHDF5(hdf5File, hdf5group_2, out_data_2);

		// import
		// data 2: strided
		HDF5ImportInfo infoHDF5_2(hdf5File, hdf5group_2);
		// import to strided array
		MultiArray< 3, RGBValue<double> > in_data_1(MultiArrayShape<3>::type(3, infoHDF5_2.shapeOfDimension(1), infoHDF5_2.shapeOfDimension(2))); // strided
		//in_data_1.init(42);
		MultiArrayView< 2, RGBValue<double>, StridedArrayTag > in_data_2(in_data_1.bindInner(0));

        readHDF5(infoHDF5_2, in_data_2);

		// compare content
		should (in_data_2 == out_data_2);
	}

	void testTinyVectorUnstridedHDF5ExportImport()
	{
		// export and import data from and to scalar unstrided array

		char hdf5File[] = "testfile_tinyvector_unstrided.hdf5";

		// data 1: 3D multi array
		// create TinyVector image from that
		MultiArray< 2, TinyVector<double, 4> > out_data_1(MultiArrayShape<2>::type(5, 8));
        // ...initialize the array to the test data
        for (int i = 0; i < 40; ++i)
            out_data_1.data () [i] = TinyVector<double, 4>(i + 0.1, i + 0.2, i + 0.3, i + 0.4);

		// export
		// ...data 2: TinyVector image
		char hdf5group_1[] = "group/subgroup/data_tinyvector";
		writeHDF5(hdf5File, hdf5group_1, out_data_1);

		// import
		// data 1: 
		HDF5ImportInfo infoHDF5_1(hdf5File, hdf5group_1);
        MultiArray< 2, TinyVector<double, 4> > in_data_1(MultiArrayShape<2>::type(infoHDF5_1.shapeOfDimension(1), infoHDF5_1.shapeOfDimension(2)));
        readHDF5(infoHDF5_1, in_data_1);

		// compare content
		should (in_data_1 == out_data_1);
	}


	void testTinyVectorStridedHDF5ExportImport()
	{
		// export and import data from and to scalar unstrided array

		char hdf5File[] = "testfile_tinyvector_strided.hdf5";

		// data 1: 3D multi array
		// create TinyVector image from that
		MultiArray< 3, TinyVector<double, 4> > out_data_1(MultiArrayShape<3>::type(5, 3, 8));
        // ...initialize the array to the test data
        for (int i = 0; i < 120; ++i)
            out_data_1.data () [i] = TinyVector<double, 4>(i + 0.1, i + 0.2, i + 0.3, i + 0.4);
		MultiArrayView< 2, TinyVector<double, 4>, StridedArrayTag> out_data_2(out_data_1.bindAt(1, 0));

		// export
		// ...data 2: TinyVector image
		char hdf5group_2[] = "group/subgroup/data_tinyvector";
		writeHDF5(hdf5File, hdf5group_2, out_data_2);

		// import
		// data 2: strided
		HDF5ImportInfo infoHDF5_2(hdf5File, hdf5group_2);
		// import to strided array
		MultiArray< 3, TinyVector<double, 4> > in_data_1(MultiArrayShape<3>::type(3, infoHDF5_2.shapeOfDimension(1), infoHDF5_2.shapeOfDimension(2))); // strided
		MultiArrayView< 2, TinyVector<double, 4>, StridedArrayTag > in_data_2(in_data_1.bindInner(0));

        readHDF5(infoHDF5_2, in_data_2);

		// compare content
		should (in_data_2 == out_data_2);
	}


	void testScalarToRGBValueUnstridedHDF5ExportImport()
	{
		// export scalar 3D array and import as RGB image (unstrided)

		char hdf5File[] = "testfile_scalar2rgbvalue_unstrided.hdf5";

		// data 1: 3D multi array
		MultiArray< 3, double > out_data_1(MultiArrayShape<3>::type(3, 5, 8));
        // ...initialize the array to the test data
        for (int i = 0; i < 120; ++i)
            out_data_1.data () [i] = i;

		// export
		// ...data 1: 
		char hdf5group_1[] = "group/subgroup/data";
		writeHDF5(hdf5File, hdf5group_1, out_data_1);

		// import
		// data 1: 
		HDF5ImportInfo infoHDF5_1(hdf5File, hdf5group_1);
        MultiArray< 2, RGBValue<double> > in_data_1(MultiArrayShape<2>::type(infoHDF5_1.shapeOfDimension(1), infoHDF5_1.shapeOfDimension(2)));
        readHDF5(infoHDF5_1, in_data_1);

		// compare content
		int i = 0;
		for (int j=0; j<120; ++i, j+=3)
			should (in_data_1.data () [i] == RGBValue<double>((double)out_data_1.data () [j], (double)out_data_1.data () [j+1], (double)out_data_1.data () [j+2]) );
	}


	void testOverwriteExistingDataInHDF5()
	{
		// write data to file, close file, open file, overwrite data, compare

		char hdf5File[] = "testfile_overwrite_existing_data.hdf5";

		// data 1: int data in 2 dimensions (partly negative)
		MultiArray<2,int> out_data_1(MultiArrayShape<2>::type(10, 11));
        // ...initialize the array to the test data
        for (int i = 0; i < 110; ++i)
            out_data_1.data () [i] = i - 55;
		// data 2: double data in 4 dimensions (partly negative)
		MultiArray<4,double> out_data_2(MultiArrayShape<4>::type(10, 2, 3, 4));
        // ...initialize the array to the test data
        for (int i = 0; i < 240; ++i)
            out_data_2.data () [i] = i + (std::rand() / (double)RAND_MAX) - 120;

		// export
		// ...data 1: 
		char hdf5group_1[] = "group/subgroup/data1";
		writeHDF5(hdf5File, hdf5group_1, out_data_1);
		// ...data 2, overwriting existing data
		writeHDF5(hdf5File, hdf5group_1, out_data_2);

		// import
		// data 1: 
		HDF5ImportInfo infoHDF5_2(hdf5File, hdf5group_1);
        MultiArray<4,double> in_data_2(MultiArrayShape<4>::type(infoHDF5_2.shapeOfDimension(0), infoHDF5_2.shapeOfDimension(1), infoHDF5_2.shapeOfDimension(2), infoHDF5_2.shapeOfDimension(3)));
        readHDF5(infoHDF5_2, in_data_2);

		// compare content
		should (in_data_2 == out_data_2);
	}

	void testAppendNewDataToHDF5()
	{
		// write data to file, close file, open file, write more data, compare

		char hdf5File[] = "testfile_append_new_data.hdf5";

		// data 1: int data in 2 dimensions (partly negative)
		MultiArray<2,int> out_data_1(MultiArrayShape<2>::type(10, 11));
        // ...initialize the array to the test data
        for (int i = 0; i < 110; ++i)
            out_data_1.data () [i] = i - 55;

		// data 2: double data in 4 dimensions (partly negative)
		MultiArray<4,double> out_data_2(MultiArrayShape<4>::type(10, 2, 3, 4));
        // ...initialize the array to the test data
        for (int i = 0; i < 240; ++i)
            out_data_2.data () [i] = i + (std::rand() / (double)RAND_MAX) - 120;

		// export
		// ...data 1
		char hdf5group_1[] = "group/data1";
		writeHDF5(hdf5File, hdf5group_1, out_data_1);

		// append to existing file
		// ...data 2
		char hdf5group_2[] = "group/subgroup/data2";
		writeHDF5(hdf5File, hdf5group_2, out_data_2);

		// import
		// ...data 1
		HDF5ImportInfo infoHDF5_1(hdf5File, hdf5group_1);
        MultiArray<2,int> in_data_1(MultiArrayShape<2>::type(infoHDF5_1.shapeOfDimension(0), infoHDF5_1.shapeOfDimension(1)));
        readHDF5(infoHDF5_1, in_data_1);
		// ...data 2
		HDF5ImportInfo infoHDF5_2(hdf5File, hdf5group_2);
        MultiArray<4,double> in_data_2(MultiArrayShape<4>::type(infoHDF5_2.shapeOfDimension(0), infoHDF5_2.shapeOfDimension(1), infoHDF5_2.shapeOfDimension(2), infoHDF5_2.shapeOfDimension(3)));
        readHDF5(infoHDF5_2, in_data_2);

		// compare content
		// ...data 1
		should (in_data_1 == out_data_1);
		// ...data 2
		should (in_data_2 == out_data_2);
	}

};

struct HDF5ImportExportTestSuite : public vigra::test_suite
{
    HDF5ImportExportTestSuite()
        : vigra::test_suite("HDF5ImportExportTestSuite")
    {
		// tests for scalar data
		add(testCase(&HDF5ExportImportTest::testScalarUnstridedHDF5ExportImport));
		add(testCase(&HDF5ExportImportTest::testScalarStridedHDF5ExportImport));

		// tests for non-scalar data
		// RGBValue
		add(testCase(&HDF5ExportImportTest::testRGBValueUnstridedHDF5ExportImport));
		add(testCase(&HDF5ExportImportTest::testRGBValueStridedHDF5ExportImport));
		// TinyVector
		add(testCase(&HDF5ExportImportTest::testTinyVectorUnstridedHDF5ExportImport));
		add(testCase(&HDF5ExportImportTest::testTinyVectorStridedHDF5ExportImport));

		// mixed forms
		add(testCase(&HDF5ExportImportTest::testScalarToRGBValueUnstridedHDF5ExportImport));

		// general tests
		add(testCase(&HDF5ExportImportTest::testOverwriteExistingDataInHDF5));
		add(testCase(&HDF5ExportImportTest::testAppendNewDataToHDF5));
	}
};


int main (int argc, char ** argv)
{
    HDF5ImportExportTestSuite test;
    const int failed = test.run(vigra::testsToBeExecuted(argc, argv));
    std::cout << test.report() << std::endl;

	return failed != 0;
}