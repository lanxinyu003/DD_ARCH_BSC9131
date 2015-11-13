=====================================================
1. Title:  IPC integration demo
!!!!!!!!!!!!!!!!!!!!!!THIS DEMO IS A SOURCE CODE FOR A TEST RELEASED BY THE LINUX BSP TEAM!!!!!!!!!!!!!!!!!!!!!!
!!!!!!!!!!!!!!!!!!!!!!IT CANNOT BE RUN INDIVIDIALLY, BUT ONLY AS A BINARY CODE OF LINUX!!!!!!!!!!!!!!!!!!!!!!

=====================================================
2. Description
IPC integration demo
sends and receives messages between cores through several channels
the following types of chnales are configured and used:
channel ID 0: SC is producer, PA is consumer - pointer channel used to notify the PA that the SC is ready
channel ID 2: PA is producer, SC is consumer - message channel with interrupts 
channel ID 3: SC is producer, PA is consumer - pointer channel with interrupts 
channel ID 4: SC is producer, PA is consumer - pointer channel without interrupts (polling)
channel ID 5: PA is producer, SC is consumer - message channel without interrupts (polling)                                                                
channel ID 6: PA is producer, SC is consumer - message channel with interrupts 
channel ID 7: SC is producer, PA is consumer - pointer channel with interrupts                                                                              
   
=====================================================
3. Build Configurations
3.1. Import the .project file to the workspace
3.3. This demo has the following target(s):
3.3.1. "IPC - Debug": is single core. Support the PSC9131 on the PSC9131 RDB
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
5.1.2 execution can be done only through Linux console
   
=====================================================
6. Results
Demo results are received by Linux console according to messages sent and received by PA and DSP.
 
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
