// ****************************************************************************
//
//    Copyright (c) 2017, Ayushi Sinha, Russell Taylor, Johns Hopkins University.
//    All rights reserved.
//
//    Redistribution and use in source and binary forms, with or without
//    modification, are permitted provided that the following conditions are
//    met:
//
//    1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
//    3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
//    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
//    HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
//    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
//    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
//    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
//    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//  
// ****************************************************************************
#ifndef _algDirICP_GDIMLOP_dlibWrapper_h
#define _algDirICP_GDIMLOP_dlibWrapper_h

#include "cisstVector.h"
#include "dlib/optimization.h"
//#include "algDirICP_GDIMLOP.h"

class algDirICP_GDIMLOP;  // forward decleration

class algDirICP_GDIMLOP_dlibWrapper
{
public:

  typedef dlib::matrix<double,0,1> dlib_vector;


  // -- Variables -- //

  double maxIter;
  double gradientNormThresh;
  //double tol_df;


  // -- Methods -- //

  // constructor
  algDirICP_GDIMLOP_dlibWrapper(algDirICP_GDIMLOP *argAlg); // algDirICP_GIMLOP *kent );

  vctDynamicVector<double>  ComputeRegistration(const vctDynamicVector<double> &x0, algDirICP_GDIMLOP *kent);
};

#endif