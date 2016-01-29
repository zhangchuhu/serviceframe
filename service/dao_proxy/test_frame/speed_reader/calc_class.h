#ifndef _CALC_CLASS_CONVERT_H_20120530_
#define _CALC_CLASS_CONVERT_H_20120530_
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <stdint.h>
#include "source_parser.h"
#include "stdpack_def.h"
#include "stdpack_cnv.h"

using namespace std;


void gl_show_result(map<uint64_t,StatIndex>& accm);
void gl_sum_to_it(map<uint64_t,StatIndex>& accm,uint64_t tim,StatIndex& si);
void gl_calc_diffcore(double lhs,double rhs,int& diff);



void calc_diffs(map<uint64_t,StatIndex>& m_sresult
	,map<uint64_t,SndDistribute>& m_dis_result
	,map<uint64_t,StatIndexEx>& m_ex);

void gl_show_result_ex(map<uint64_t,StatIndexEx>& accm,ostream& out_srm);







#endif	// _CALC_CLASS_CONVERT_H_20120530_

