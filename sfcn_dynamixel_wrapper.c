

/*
 * Include Files
 *
 */
#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif

/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
#ifndef MATLAB_MEX_FILE
	#include <Arduino.h>
	boolean ini = false;
	#define N 1
	int oldq[6];
#endif
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 1
/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
#ifndef MATLAB_MEX_FILE

	/* data = dynamixel_com(packet_to_read, packet_dimension, port_number, baud_rate, delay_time)
	   This function is used to communicate with dynamixel servos, sending the packets and returning
	   the information from them if it is required.
	*/
	
	unsigned int dynamixel_com(unsigned char *packet, int dim, int p, unsigned long baud,
							   unsigned int delay) {
		
		unsigned char sum = 0;			// calculated Checksum
		unsigned char check;			// received Checksum (Dynamixel)
		unsigned char data_l = 0;		// low byte of the read information
		unsigned char data_h = 0;		// high byte of the read information
		unsigned char error;			// error (Dynamixel)
		unsigned int data;				// read information

		// calculating CheckSum (Dynamixel) to send
		for (int i=0; i<dim; i++) {
			sum = sum + packet[i];
		}
		sum = ~(sum);
		
		// checking port
		switch (p) {
		
			case 1: // ---------- Serial1 ----------
				// initializing serial communication
				if (ini==false) {
					Serial1.begin(baud);
					ini = true;
				}
				
				// sending reading packet
				Serial1.write(255);							// 0xFF
				Serial1.write(255);							// 0xFF
				for (int i=0; i<dim; i++) {
					Serial1.write(*(packet+i));
				}
				Serial1.write(sum);
				delayMicroseconds(delay);
				
				// reading data
				if (Serial1.available()) {
					Serial1.read();             			// 0xFF
					Serial1.read();             			// 0xFF
					sum = Serial1.read() + Serial1.read();  // ID + length (Dynamixel)
					error = Serial1.read();     			// error (Dynamixel)
					data_l = Serial1.read();    			// data (LOW)
					if (Serial1.available()>1) {
						data_h = Serial1.read();    		// data (HIGH)
					}
					check = Serial1.read();       			// CheckSum (Dynamixel)
				}
				break;

				
			case 2: // ---------- Serial2 ----------
				// initializing serial communication
				if (ini==false) {
					Serial2.begin(baud);
					ini = true;
				}

				// sending reading packet
				Serial2.write(255);							// 0xFF
				Serial2.write(255);							// 0xFF
				for (int i=0; i<dim; i++) {
					Serial2.write(*(packet+i));
				}
				Serial2.write(sum);
				delayMicroseconds(delay);
				
				// reading data
				if (Serial2.available()) {
					Serial2.read();             			// 0xFF
					Serial2.read();             			// 0xFF
					sum = Serial2.read() + Serial2.read();  // ID + length (Dynamixel)
					error = Serial2.read();     			// error (Dynamixel)
					data_l = Serial2.read();    			// data (LOW)
					if (Serial2.available()>1) {
						data_h = Serial2.read();    		// data (HIGH)
					}
					check = Serial2.read();       			// CheckSum (Dynamixel)
				}
				break;

				
			case 3: // ---------- Serial3 ----------
				// initializing serial communication
				if (ini==false) {
					Serial3.begin(baud);
					ini = true;
				}
				
				// sending reading packet
				Serial3.write(255);							// 0xFF
				Serial3.write(255);							// 0xFF
				for (int i=0; i<dim; i++) {
					Serial3.write(*(packet+i));
				}
				Serial3.write(sum);
				delayMicroseconds(delay);
				
				// reading data
				if (Serial3.available()) {
					Serial3.read();             			// 0xFF
					Serial3.read();             			// 0xFF
					sum = Serial3.read() + Serial3.read();  // ID + length (Dynamixel)
					error = Serial3.read();     			// error (Dynamixel)
					data_l = Serial3.read();    			// data (LOW)
					if (Serial3.available()>1) {
						data_h = Serial3.read();    		// data (HIGH)
					}
					check = Serial3.read();       			// CheckSum (Dynamixel)
				}
				break;
				
		}

		sum = ~(sum + error + data_l + data_h);
		
		// checking both CheckSum
		if (sum!=check) {
			bitSet(error,7);    // if different, set bit 7
		}
		
		// returning data
		data = data_h << 8;
		return data + data_l;

	}
	
#endif
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Output functions
 *
 */
void sfcn_dynamixel_Outputs_wrapper(const int16_T *qd,
			const int16_T *dqd,
			int16_T *q,
			int16_T *load,
			const real_T *xD,
			const int8_T  *port, const int_T  p_width0,
			const uint32_T  *bd, const int_T  p_width1,
			const int8_T  *id, const int_T  p_width2,
			const uint32_T  *t, const int_T  p_width3)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
if (xD[0]==1) {
	
#ifndef MATLAB_MEX_FILE

    unsigned char toread[5]; 	// instruction to read servos
	unsigned int goal, veloc;	
	bool cambio;		// goal position

	
	// read position
	for (int i=0; i<N; i++) {
			
		// ---------- reading position ----------
		toread[0] = id[i];	// ID (Dynamixel)
		toread[1] = 4;       // length (Dynamixel)
		toread[2] = 2;       // READ_DATA (instruction)
		toread[3] = 36;      // start address of data to be read (0x24)
		toread[4] = 2;       // length of data to be read
				
		q[i]=dynamixel_com((unsigned char*)toread,5,port[0],bd[0],t[0]); // present angle
				
		// ---------- reading load ----------
		toread[3] = 40;					// start address of data to be read (0x28)
		load[i] = dynamixel_com((unsigned char*)toread,5,port[0],bd[0],t[0]); // load						
	}
			

    
    
	// ---------- input change ----------
	for (int i=0; i<N; i++) {		
		if (qd[i] != oldq[i])			// if input has changed
			cambio =1;
		oldq[i]=qd[i];
        }
	// ---------- sending orders ----------
	if (cambio) {		
		unsigned char length = 5*N + 4;	// length (Dynamixel)
		int size = length + 1;				// size of the packet
		unsigned char towrite[size];    	// packet to write position
		unsigned char data_l, data_h;		// data low and high
		int k = 5;							// index for the packet
		
		towrite[0] = 254;					// 0xFE (ID)
		towrite[1] = length;				// length (Dynamixel)
		towrite[2] = 131;					// 0x83 (instruction)
		towrite[3] = 30;					// start address to write data(Goal Position(L))
		towrite[4] = 4;						// length of data to write
		
		for ( int i=0; i<N; i++) {		
						towrite[k] = id[i];	// ID (Dynamixel)
						k++;
						goal=qd[i];
						data_l = goal;						// goal position (LOW)
						data_h = goal >> 8;				// goal position (HIGH)
						towrite[k] = data_l;
						k++;
						towrite[k] = data_h;
						k++;
						
						veloc=dqd[i];			
						data_l = veloc;					// goal speed (LOW)
						data_h = veloc >> 8;			// goal speed (HIGH)
						towrite[k] = data_l;
						k++;
						towrite[k] = data_h;
						k++;
					}
							
		// sending packet
		dynamixel_com((unsigned char*)towrite,size,port[0],bd[0],t[0]);
	}


#endif
	
}
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}

/*
  * Updates function
  *
  */
void sfcn_dynamixel_Update_wrapper(const int16_T *qd,
			const int16_T *dqd,
			int16_T *q,
			int16_T *load,
			real_T *xD,
			const int8_T  *port,  const int_T  p_width0,
			const uint32_T  *bd,  const int_T  p_width1,
			const int8_T  *id,  const int_T  p_width2,
			const uint32_T  *t,  const int_T  p_width3)
{
  /* %%%-SFUNWIZ_wrapper_Update_Changes_BEGIN --- EDIT HERE TO _END */
if (xD[0]!=1) {
	
#ifndef MATLAB_MEX_FILE

	unsigned char length = 5*N + 4; // length (Dynamixel)
	int size = length + 1;          // size of the packet
	unsigned char conf[size];       // configuration packet


	int k = 5;						// index for the packets
	
	conf[0] = 254;                	// 0xFE (ID)
	conf[1] = length;             	// length (Dynamixel)
	conf[2] = 131;                	// 0x83 (instruction)
	conf[4] = 4;                  	// length of data to write
		
	
	// ---------- activating torque ----------
	length = 2*N + 4;
	size = length + 1;
	conf[1] = length;			// length (Dynamixel)
	conf[3] = 24;               // start address to write data (Torque Enable)
	conf[4] = 1;				// length of data to write
	k=5;
	for (int i=0; i<N; i++) {
		conf[k] = id[i];
		k++;
		conf[k] = 1;			// torque enabled
		k++;
	}
	
	// sending packet
	dynamixel_com((unsigned char*)conf,size,port[0],bd[0],t[0]);
	
#endif
	
	xD[0]=1;
	
}
/* %%%-SFUNWIZ_wrapper_Update_Changes_END --- EDIT HERE TO _BEGIN */
}
