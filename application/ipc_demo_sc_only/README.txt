=====================================================
1. Title:  IPC starcore-only demo

=====================================================
2. Description
This demo demonstrates usage of the IPC module. it is designed to use only starcore
cores, and can be used to test the module without usage of linux.
messages are being sent from the core to itself.
 
=====================================================
3. Build Configurations
3.1. Import the .project file to the workspace
3.3. This demo has the following target(s):
3.3.1. "IPC Demo 9x31 - Debug": is single core. Support the PSC9131 on the PSC9131 RDB
3.3.1. "IPC Demo 9x32 - Debug": is single core. Support the PSC9132 on the PSC9132 QDS
3.3. Build the required target, and if necessary, the underlying libraries

=====================================================
4. Board Setup
This demo has targets for the following board(s):
4.1 PSC9131 RDB and PSC9132 QDS
4.1.1. Configure the board according to the Quick Start guide.
4.1.2. Connect a eUTAP between the PC and the board. 
4.1.3. No additional board setup is required for this demo

=====================================================
5. Execution
5.1.1 In order to execute the target described in section 3.3.1, use the following launch file(s):
5.1.1.1 ipc_demo_psc9x31_c0.launch
5.1.2 No additional setup is required on the PC for this demo
  
5.2.1 In order to execute the target described in section 3.3.1, use the following launch file(s):
5.2.1.1 ipc_demo_psc9x32_c0.launch
5.2.2 No additional setup is required on the PC for this demo
  
=====================================================
6. Results
If the execution is successful:        
 - The core stops on OS_TEST_PASSED
 
=====================================================
7. Optional Modifications 
7.1 Linker file modifications:
7.1.1 In order to enable compiler rtlib functionalities, set USING_RTLIB to 1 in the linker command file (lcf)
7.1.2 In order to enable application specified Kernel Awareness stack location, set USING_USER_KA_STACK to 1 in the lcf. 
          The user can also modify the location and size of the stack in the lcf 
7.1.3 In order to enable automatic VTB configuration, set USING_VTB to 1 in the lcf
7.2 Demo modifications
The user can define in the app_config.h CONTINUOUS_TEST in order to run the test in a continous mode (no end) 
=====================================================
8. Suggested Reading
8.1 <CodeWarrior  Location>\SC\Help\PDF\SmartDSP OS User Guide.pdf
8.2 <CodeWarrior  Location>\SC\Help\PDF\SmartDSP OS API Reference Manual.pdf
