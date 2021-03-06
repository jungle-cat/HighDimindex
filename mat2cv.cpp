/*
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2012, Willow Garage, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   * Neither the name of Willow Garage, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 *  File:    main.cpp
 *  Author:  Hilton Bristow
 *  Created: Jun 27, 2012
 */

/*! \mainpage Matlab Mat File Parser for C++ OpenCV
 *
 * MatlabIO provides the ability to read .Mat files generated by Matlab
 * into C++ programs. MatlabIO stores matrices in the OpenCV cv::Mat format
 * and is intrinsically designed for computer vision research and applications,
 * though other fields may find use in the features it provides. It has a simple,
 * easy to use interface for parsing files, searching for particular variables
 * and extracting the data in OpenCV native formats.
 */

#include <string>
#include <iostream>
#include <opencv2/core/core.hpp>
#include "third_party/include/MatlabIO.hpp"
#include "third_party/include/MatlabIOContainer.hpp"
using namespace cv;
using namespace std;

int main(int argc, char **argv) {

    // get the Matlab .Mat file from the command line
    string filename("E:/Research/Fast Indexing/ksh_code/ksh_48.mat");

    // create a new reader
    MatlabIO matio;
    bool ok = matio.open(filename, "r");
    if (!ok) return -1;

    // read all of the variables in the file
    vector<MatlabIOContainer> variables;
    variables = matio.read();

    // close the file
    matio.close();

    // display the file info
    matio.whos(variables);

    auto v = MatlabIO::convert2Map(variables);
    auto tanchors = boost::any_cast<Mat&>(v["anchor"]);
    auto tmvec = boost::any_cast<Mat&>(v["mvec"]);
    auto tA1 = boost::any_cast<Mat&>(v["A1"]);
    auto msigma = boost::any_cast<Mat&>(v["sigma"]);
    float sigma = msigma.at<double>(0);

    cout << sigma << endl;
    Mat anchors, mvec, A1;
    tanchors.convertTo(anchors, DataType<float>::type);
    tmvec.convertTo(mvec, DataType<float>::type);
    tA1.convertTo(A1, DataType<float>::type);

    FileStorage fs("config.yaml", FileStorage::WRITE);
    if (not fs.isOpened()) {
    	cerr << "Error: failed to open config file `config.yaml`!" << endl;
    	return 1;
    }

    fs << "anchors" << anchors;
    fs << "mvec" << mvec;
    fs << "A1" << A1;


    // search for a variable named "im" and "gray"

    return 0;
}
