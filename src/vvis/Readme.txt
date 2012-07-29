To build a usable libboost_thread.dylib
1. In $(BOOST_ROOT)/libs/thread/build
2. Edit Jamfile
   Add the <linkflags> option as shown below. Note that this option does not work with debug
   dll boost_thread
        : <template>thread_libs ../src/$(CPP_SOURCES).cpp
        : <define>BOOST_THREAD_BUILD_DLL=1
          <linkflags>"-Wl,-dylib_install_name,@executable_path/../Frameworks/libboost_thread.dylib"
    ;
