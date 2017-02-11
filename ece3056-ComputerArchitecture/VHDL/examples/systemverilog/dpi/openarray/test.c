#include "dpiheader.h" 

void array_query(const char* str, const svOpenArrayHandle hin)
{
   int dim, count = svDimensions(hin);

   for (dim = count; dim > 0; --dim) {
      int left  =  svLeft(hin, dim);
      int right =  svRight(hin, dim);
      int low   =  svLow(hin, dim);
      int high  =  svHigh(hin, dim);
      int incr  =  svIncrement(hin, dim);
      int size  =  svSize(hin, dim);

      vpi_printf("%s: dim:%d, incr:%d, size:%d, left:%d, right:%d, low:%d, high:%d \n",
		 str, dim, incr, size, left, right, low, high);
   }
}

void f0(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int count = svSize(hin, 1); 
    MyType *s = (MyType *)svGetArrayPtr(hin); 
    MyType *d = (MyType *)svGetArrayPtr(hout); 

    array_query("f0", hin);

    if (s && d) { /* both arrays have C layout */ 

        /* an efficient solution using pointer arithmetic */ 
        while (count--) 
            *d++ = *s++; 
    }
}

void f1(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int count = svSize(hin, 1); 
    MyType *s = (MyType *)svGetArrayPtr(hin); 
    MyType *d = (MyType *)svGetArrayPtr(hout); 

    if (s && d) { /* both arrays have C layout */ 
       /* even more efficient: */
       memcpy(d, s, svSizeOfArray(hin)); 
    }
}

void f2(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int count = svSize(hin, 1); 
    MyType *s = (MyType *)svGetArrayPtr(hin); 
    MyType *d = (MyType *)svGetArrayPtr(hout); 

    if (s && d) { /* both arrays have C layout */ 
        int i = svLeft(hin, 1); 
        int j = svLeft(hout, 1); 
	int incr_i;
	int incr_j;

	if (i == svLow(hin, 1)) {
	  incr_i = 1;
	} else {
	  incr_i = 0;
	}

	if (j == svLow(hin, 1)) {
	  incr_j = 1;
	} else {
	  incr_j = 0;
	}

        while (1) { 
           *(MyType *)svGetArrElemPtr1(hout, j) = *(MyType *)svGetArrElemPtr1(hin, i); 
	   if (i == svRight(hin, 1)) {
	       break;
	   }
	   i = incr_i ? i + 1 : i - 1;
	   j = incr_j ? j + 1 : j - 1;
        } 
     } 
} 

void f3a(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 1;

    int count = svSize(hin, dim); 
    svBitVecVal* s = (svBitVecVal*) svGetArrayPtr(hin); 
    svBitVecVal* d = (svBitVecVal*) svGetArrayPtr(hout); 

    array_query("f3", hin);

    if (s && d) { /* both arrays have C layout */ 
        int i = svLeft(hin, dim); 
        int j = svLeft(hout, dim); 
	int incr_i;
	int incr_j;
	if (i == svLow(hin, dim)) {
	  incr_i = 1;
	} else {
	  incr_i = 0;
	}	
	if (j == svLow(hout, dim)) {
	  incr_j = 1;
	} else {
	  incr_j = 0;
	}
        while (1) { 
            s = (svBitVecVal*)svGetArrElemPtr1(hin, i); 
	    svPutBitArrElem1VecVal(hout, s, j);
            if (i == svRight(hin, dim)) {
	       break;
            }
	    i = incr_i ? i + 1 : i - 1;
	    j = incr_j ? j + 1 : j - 1;
	}
    }
}

void f3b(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 1;
    svBitVecVal s;
    int i = svLeft(hin, dim); 
    int j = svLeft(hout, dim); 
    int incr_i;
    int incr_j;
    if (i == svLow(hin, dim)) {
	incr_i = 1;
    } else {
        incr_i = 0;
    }
    if (j == svLow(hout, dim)) {
	incr_j = 1;
    } else {
        incr_j = 0;
    }
    while (1) { 
       svGetBitArrElem1VecVal(&s, hin, i);
       svPutBitArrElem1VecVal(hout, &s, j);
       if (i == svRight(hin, dim)) {
           break;
       }
       i = incr_i ? i + 1 : i - 1;
       j = incr_j ? j + 1 : j - 1;
    }
}

void f4a(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 2;

    int count = svSize(hin, dim); 
    svBitVecVal* s = (svBitVecVal*) svGetArrayPtr(hin); 
    svBitVecVal* d = (svBitVecVal*) svGetArrayPtr(hout); 

    array_query("f4", hin);

    if (s && d) { /* both arrays have C layout */ 
        int i = svLeft(hin, dim);
        int incr_i;
        int j = svLeft(hout, dim);
        int incr_j;
	if (i == svLow(hin, dim)) {
	    incr_i = 1;
	} else {
            incr_i = 0;
	}
	if (j == svLow(hout, dim)) {
	    incr_j = 1;
	} else {
	    incr_j = 0;
	}
        while (1) {
            s = (svBitVecVal*)svGetArrElemPtr2(hin, 0, i); 
	    svPutBitArrElem2VecVal(hout, s, 0, j);
            if (i == svRight(hin, dim)) {
                break;
            }
            i = incr_i ? i + 1 : i - 1;
	    j = incr_j ? j + 1 : j - 1;
	}
    }
}

void f4b(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 2;
    svBitVecVal s;
    int i = svLeft(hin, dim); 
    int incr_i;
    int j = svLeft(hout, dim);
    int incr_j;
    if (j == svLow(hout, dim)) {
	incr_j = 1;
    } else {
        incr_j = 0;
    }
    if (i == svLow(hin, dim)) {
	incr_i = 1;
    } else {
        incr_i = 0;
    }
    while (1) {
       svGetBitArrElem2VecVal(&s, hin, 0, i);
       svPutBitArrElem2VecVal(hout, &s, 0, j);
       if (i == svRight(hin, dim)) {
           break;
       }
       i = incr_i ? i + 1 : i - 1;
       j = incr_j ? j + 1 : j - 1;
    }
}


void f5a(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 3;

    int count = svSize(hin, dim); 
    svBitVecVal* s = (svBitVecVal*) svGetArrayPtr(hin); 
    svBitVecVal* d = (svBitVecVal*) svGetArrayPtr(hout); 

    array_query("f5", hin);

    if (s && d) { /* both arrays have C layout */ 
        int i = svLeft(hin, dim); 
        int incr_i;
	int j = svLeft(hout, dim);
	int incr_j;
	if (j == svLow(hout, dim)) {
	  incr_j = 1;
	} else {
	  incr_j = 0;
	}
	if (i == svLow(hin, dim)) {
	    incr_i = 1;
	} else {
            incr_i = 0;
	}
        while (1) { 
            s = (svBitVecVal*)svGetArrElemPtr3(hin, 0, 0, i); 
	    svPutBitArrElem3VecVal(hout, s, 0, 0, j);
            if (i == svRight(hin, dim)) {
                break;
            }
            i = incr_i ? i + 1 : i - 1;
	    j = incr_j ? j + 1 : j - 1;
	}
    }
}

void f5b(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 3;
    svBitVecVal s;
    int i = svLeft(hin, dim); 
    int incr_i;
    int j = svLeft(hout, dim);
    int incr_j;
    if (j == svLow(hout, dim)) {
	incr_j = 1;
    } else {
        incr_j = 0;
    }
    if (i == svLow(hin, dim)) {
	incr_i = 1;
    } else {
        incr_i = 0;
    }
    while (1) { 
       svGetBitArrElem3VecVal(&s, hin, 0, 0, i);
       svPutBitArrElem3VecVal(hout, &s, 0, 0, j);
       if (i == svRight(hin, dim)) {
           break;
       }
       i = incr_i ? i + 1 : i - 1;
       j = incr_j ? j + 1 : j - 1;
    }
}


void f6a(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 1;

    int count = svSize(hin, dim); 
    svLogicVecVal* s = (svLogicVecVal*) svGetArrayPtr(hin); 
    svLogicVecVal* d = (svLogicVecVal*) svGetArrayPtr(hout); 

    array_query("f6", hin);

    if (s && d) { /* both arrays have C layout */ 
        int i = svLeft(hin, dim); 
        int incr_i;
	int j = svLeft(hout, dim);
	int incr_j;
	if (j == svLow(hout, dim)) {
	  incr_j = 1;
	} else {
	  incr_j = 0;
	}
	if (i == svLow(hin, dim)) {
	    incr_i = 1;
	} else {
            incr_i = 0;
	}
        while (1) { 
            s = (svLogicVecVal*)svGetArrElemPtr1(hin, i); 
 	    svPutLogicArrElem1VecVal(hout, s, j);
           if (i == svRight(hin, dim)) {
                break;
            }
            i = incr_i ? i + 1 : i - 1;
	    j = incr_j ? j + 1 : j - 1;
	}
    }
}

void f6b(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 1;
    svLogicVecVal s;
    int i = svLeft(hin, dim); 
    int incr_i;
    int j = svLeft(hout, dim);
    int incr_j;
    if (j == svLow(hout, dim)) {
	incr_j = 1;
    } else {
        incr_j = 0;
    }
    if (i == svLow(hin, dim)) {
	incr_i = 1;
    } else {
        incr_i = 0;
    }
    while (1) {
       svGetLogicArrElem1VecVal(&s, hin, i);
       svPutLogicArrElem1VecVal(hout, &s, j);
       if (i == svRight(hin, dim)) {
           break;
       }
       i = incr_i ? i + 1 : i - 1;
       j = incr_j ? j + 1 : j - 1;
    }
}

void f7a(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 2;

    int count = svSize(hin, dim); 
    svLogicVecVal* s = (svLogicVecVal*) svGetArrayPtr(hin); 
    svLogicVecVal* d = (svLogicVecVal*) svGetArrayPtr(hout); 

    array_query("f7", hin);

    if (s && d) { /* both arrays have C layout */ 
        int i = svLeft(hin, dim); 
        int incr_i;
	int j = svLeft(hout, dim);
	int incr_j;
	if (j == svLow(hout, dim)) {
	    incr_j = 1;
	} else {
	    incr_j = 0;
	}
	if (i == svLow(hin, dim)) {
	    incr_i = 1;
	} else {
            incr_i = 0;
	}
        while (1) { 
            s = (svLogicVecVal*)svGetArrElemPtr2(hin, 0, i); 
	    svPutLogicArrElem2VecVal(hout, s, 0, j);
            if (i == svRight(hin, dim)) {
                break;
            }
            i = incr_i ? i + 1 : i - 1;
	    j = incr_j ? j + 1 : j - 1;
	}
    }
}

void f7b(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 2;
    svLogicVecVal s;
    int i = svLeft(hin, dim); 
    int incr_i;
    int j = svLeft(hout, dim);
    int incr_j;
    if (j == svLow(hout, dim)) {
	incr_j = 1;
    } else {
        incr_j = 0;
    }
    if (i == svLow(hin, dim)) {
	incr_i = 1;
    } else {
        incr_i = 0;
    }
    while (1) {
       svGetLogicArrElem2VecVal(&s, hin, 0, i);
       svPutLogicArrElem2VecVal(hout, &s, 0, j);
       if (i == svRight(hin, dim)) {
           break;
       }
       i = incr_i ? i + 1 : i - 1;
       j = incr_j ? j + 1 : j - 1;
    }
}


void f8a(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 3;

    int count = svSize(hin, dim); 
    svLogicVecVal* s = (svLogicVecVal*) svGetArrayPtr(hin); 
    svLogicVecVal* d = (svLogicVecVal*) svGetArrayPtr(hout); 

    array_query("f8", hin);

    if (s && d) { /* both arrays have C layout */ 
        int i = svLeft(hin, dim); 
        int incr_i;
	int j = svLeft(hout, dim);
	int incr_j;
	if (j == svLow(hout, dim)) {
	  incr_j = 1;
	} else {
	  incr_j = 0;
	}
	if (i == svLow(hin, dim)) {
	    incr_i = 1;
	} else {
            incr_i = 0;
	}
        while (1) { 
            s = (svLogicVecVal*)svGetArrElemPtr3(hin, 0, 0, i); 
	    svPutLogicArrElem3VecVal(hout, s, 0, 0, j);
            if (i == svRight(hin, dim)) {
                break;
            }
            i = incr_i ? i + 1 : i - 1;
	    j = incr_j ? j + 1 : j - 1;
	}
    }
}

void f8b(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 3;
    svLogicVecVal s;
    int i = svLeft(hin, dim); 
    int incr_i;
    int j = svLeft(hout, dim);
    int incr_j;
    if (j == svLow(hout, dim)) {
	incr_j = 1;
    } else {
        incr_j = 0;
    }
    if (i == svLow(hin, dim)) {
	incr_i = 1;
    } else {
        incr_i = 0;
    }
    while (1) {
       svGetLogicArrElem3VecVal(&s, hin, 0, 0, i);
       svPutLogicArrElem3VecVal(hout, &s, 0, 0, j);
       if (i == svRight(hin, dim)) {
           break;
       }
       i = incr_i ? i + 1 : i - 1;
       j = incr_j ? j + 1 : j - 1;
    }
}


void f9a(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 1;
    svBit s;
    int i = svLeft(hin, dim); 
    int incr_i;
    int j = svLeft(hout, dim);
    int incr_j;
    if (j == svLow(hout, dim)) {
	incr_j = 1;
    } else {
        incr_j = 0;
    }
    array_query("f9", hin);
    if (i == svLow(hin, dim)) {
	incr_i = 1;
    } else {
        incr_i = 0;
    }
    while (1) {
       s = svGetBitArrElem1(hin, i);
       svPutBitArrElem1(hout, s, j);
       if (i == svRight(hin, dim)) {
           break;
       }
       i = incr_i ? i + 1 : i - 1;
       j = incr_j ? j + 1 : j - 1;
    }    
}

void f9b(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 2;
    svBit s;
    int i = svLeft(hin, dim); 
    int incr_i;
    int j = svLeft(hout, dim);
    int incr_j;
    if (j == svLow(hout, dim)) {
	incr_j = 1;
    } else {
        incr_j = 0;
    }
    if (i == svLow(hin, dim)) {
	incr_i = 1;
    } else {
        incr_i = 0;
    }
    while (1) {
       s = svGetBitArrElem2(hin, 0, i);
       svPutBitArrElem2(hout, s, 0, j);
       if (i == svRight(hin, dim)) {
           break;
       }
       i = incr_i ? i + 1 : i - 1;
       j = incr_j ? j + 1 : j - 1;
    }    
}

void f9c(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 3;
    svBit s;
    int i = svLeft(hin, dim); 
    int incr_i;
    int j = svLeft(hout, dim);
    int incr_j;
    if (j == svLow(hout, dim)) {
	incr_j = 1;
    } else {
        incr_j = 0;
    }
    if (i == svLow(hin, dim)) {
	incr_i = 1;
    } else {
        incr_i = 0;
    }
    while (1) {
       s = svGetBitArrElem3(hin, 0, 0, i);
       svPutBitArrElem3(hout, s, 0, 0, j);
       if (i == svRight(hin, dim)) {
           break;
       }
       i = incr_i ? i + 1 : i - 1;
       j = incr_j ? j + 1 : j - 1;
    }    
}


void f10a(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 1;
    svLogic s;
    int i = svLeft(hin, dim); 
    int incr_i;
    int j = svLeft(hout, dim);
    int incr_j;
    if (j == svLow(hout, dim)) {
	incr_j = 1;
    } else {
        incr_j = 0;
    }
    array_query("f10", hin);
    if (i == svLow(hin, dim)) {
	incr_i = 1;
    } else {
        incr_i = 0;
    }
    while (1) {
       s = svGetLogicArrElem1(hin, i);
       svPutLogicArrElem1(hout, s, j);
       if (i == svRight(hin, dim)) {
           break;
       }
       i = incr_i ? i + 1 : i - 1;
       j = incr_j ? j + 1 : j - 1;
    }    
}

void f10b(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 2;
    svLogic s;
    int i = svLeft(hin, dim); 
    int incr_i;
    int j = svLeft(hout, dim);
    int incr_j;
    if (j == svLow(hout, dim)) {
	incr_j = 1;
    } else {
        incr_j = 0;
    }
    if (i == svLow(hin, dim)) {
	incr_i = 1;
    } else {
        incr_i = 0;
    }
    while (1) {
       s = svGetLogicArrElem2(hin, 0, i);
       svPutLogicArrElem2(hout, s, 0, j);
       if (i == svRight(hin, dim)) {
           break;
       }
       i = incr_i ? i + 1 : i - 1;
       j = incr_j ? j + 1 : j - 1;
    }    
}

void f10c(const svOpenArrayHandle hin, const svOpenArrayHandle hout) 
{ 
    int dim = 3;
    svLogic s;
    int i = svLeft(hin, dim); 
    int incr_i;
    int j = svLeft(hout, dim);
    int incr_j;
    if (j == svLow(hout, dim)) {
	incr_j = 1;
    } else {
        incr_j = 0;
    }
    if (i == svLow(hin, dim)) {
	incr_i = 1;
    } else {
        incr_i = 0;
    }
    while (1) {
       s = svGetLogicArrElem3(hin, 0, 0, i);
       svPutLogicArrElem3(hout, s, 0, 0, j);
       if (i == svRight(hin, dim)) {
           break;
       }
       i = incr_i ? i + 1 : i - 1;
       j = incr_j ? j + 1 : j - 1;
    }    
}
