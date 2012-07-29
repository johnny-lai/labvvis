include $(THESIS_PATH)/makefiles/preamble.make

APPS=time

time_CXX=g++
time_APPS=\
	implementation \
	arithmetic_vp \
	arithmetic_novp \
	arithmetic_altivec \
#          bitwise \
          comparison \
          functional \
          info \
          logical \
          traits \
          vector_cast
time_APP_PATH=build
time_CFLAGS=-Os $(COMMON_CFLAGS) \
	$(BOOST_CFLAGS) \
	$(VVM_CFLAGS) -Winline --param max-inline-insns-single=2000 --param large-function-growth=2000
#	-I$(THESIS_PATH)/projects/vvm/include_func 
#	-I$(THESIS_PATH)/projects/vvm/include_expr2
#	-I$(THESIS_PATH)/projects/vvm/include_expr
time_LIB=$(COMMON_LIB) \
	$(VVM_LIB)
time_TEST_OUT_PATH=tout
time_TEST_IN=tout/time.tin

#### arithmetic ####
arithmetic_novp_CFLAGS=-Iinclude/arithmetic
arithmetic_novp_CXX_OBJ=symlink/arithmetic_novp.o
arithmetic_novp_JGRAPH_OUT=graph/arithmetic.eps
arithmetic_novp_APP_PATH=$(time_novp_APP_PATH)
arithmetic_novp_TEST_IN=$(time_novp_TEST_IN)
arithmetic_novp_TEST_OUT_PATH=$(time_novp_TEST_OUT_PATH)
arithmetic_novp_TEST_IN=tout/scalar.tin

arithmetic_vp_CFLAGS=-Iinclude/arithmetic -maltivec
arithmetic_vp_VEC_OBJ=symlink/arithmetic_vp.o
arithmetic_vp_TEST_IN=tout/vp.tin

arithmetic_altivec_CFLAGS=-Iinclude/arithmetic -maltivec
arithmetic_altivec_VEC_OBJ=symlink/arithmetic_altivec.o
arithmetic_altivec_TEST_IN=tout/vp.tin

implementation_CXX_OBJ=implementation.o

symlink/arithmetic_vp.cpp \
symlink/arithmetic_novp.cpp: main.cpp
	ln -s ../main.cpp $@

symlink/arithmetic_altivec.cpp: altivec.cpp
	ln -s ../altivec.cpp $@

include $(THESIS_PATH)/makefiles/postamble.make
