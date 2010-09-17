#ifndef _TPX_GAIN_HH_
#define _TPX_GAIN_HH_

#include <stdio.h>

#include "tpxCore.h"

// clock types aka frequencies
#define TPX_CLOCK_TCU_LOC	9216000
#define TPX_CLOCK_TCU_RHIC	9370000	// mean of all
#define TPX_CLOCK_TCD		9434800	// ?



// default is TCU local
#define TPX_TCU_LOC_PED			92
#define TPX_TCU_LOC_START		97
#define TPX_TCU_LOC_STOP		101
#define TPX_TCU_LOC_TIME_0		98.668442	// measured with run 10346005

// in the (rare) case we are running with RHIC I use some average
#define TPX_TCU_RHIC_PED		92
#define TPX_TCU_RHIC_START		98
#define TPX_TCU_RHIC_STOP		102
#define TPX_TCU_RHIC_TIME_0		99.0		// some number....



/* FY10 and previous value.
   In FY11 this will change due to:
   - longer cable from the TCD to the distribution box
   - new TCD
*/
#define TPX_TCD_PED			92
#define TPX_TCD_START			100
#define TPX_TCD_STOP			103
#define TPX_TCD_TIME_0			100.84	// or perhaps 100.90?


// accepted region: start must be the lowest, stop must be the highest
#define TPX_PULSER_PED_START	92
#define TPX_PULSER_PED_STOP	103

class tpxGain
{
public:
	tpxGain() ;
	~tpxGain() ;

	
	struct gains {
		float g ;
		float t0 ;
	} ;

	// used in running
	struct gains *gains[24] ;	// pointers to sector contribs, [46][183]

	void set_gains(int s, int r, int p, float g, float t0) {
		struct gains *gs = get_gains(s,r,p) ;

		gs->g = g ;
		gs->t0 = t0 ;
	}

	struct gains *get_gains(int s, int r, int p) {
		return (gains[s-1] + r*182 + (p-1)) ;
	}

	// [sector 1-24][RDO 1-6][fee-index 0-35]
	u_int bad_fee[25][7][37] ;	// [x][y][36] contains the count!

	// sector 1..24
	u_int bad_rdo_mask[25] ;	// bad if set to 1

	void if_file() ;

	char *raw_gains_fname ;

	// below used while calculating only
	struct aux {
		short low_pulse ;	// count of low gain
		short noise ;		// count of noisy
		short high_pulse ;	// count of pulse to high
		//short wrong_peak ;	// hm?
		short cou ;		// count of good events
		short need ;		// count of expected events!
		u_int adc_store[20] ;
	} *aux	;	// [24][46][182]

	struct aux *get_aux(int s, int r, int p) {
		return (aux + (s-1)*46*182 + r*182 + (p-1)) ;
	} ;

	struct means {
		double g ;
		double t0 ;
		double g_rms ;
		double t0_rms ;

	} *means ;	// [24][46]

	struct means *get_means(int s, int r) {
		return (means + (s-1)*46 + r) ;
	} ;
	

	struct fee_found_t {
		u_int got_one ;
		int ch_count[256][16] ;
	} *fee_found ;

	struct fee_found_t *get_fee_found(int s, int rdo) {
		return (fee_found + (s-1)*7 + rdo) ;
	}

	int from_file(char *fname, int sector = 0) ;

	// below used only during calculation
	int to_file(char *fname) ;

	void init(int sec=0) ;			// zap's structs; assume all TPX
	void accum(char *evbuff, int bytes) ;	// parses one RDO's worth of pulser data
	void ev_done() ;			// marks an event done...

	void calc() ;				// calculates gains and bad pads

	void compare(char *fname, int mysec=0) ;			// compares in memory image with a given file
	int summarize(char *fname, FILE *log_file=0) ;			// prints a summary of bad channels to stdout

	void free_store() ;

	u_int c_date ;	// date of file as YYYYMMDD
	u_int c_time ;	// time of file as HHMMSS
	u_int c_run ;	// run of file

	int pulser_ped ;	// first timebin with ped=0
	int pulser_start ;	// timebin with peak
	int pulser_stop ;	// last timebin which we use for the mean
	double pulser_time_0 ;

	void clock_mode(int mode) ;

private:
	void do_default(int sector) ;

	time_t	load_time ;

	int tpx_pulser_peak[25][46] ;

	int sector ;
	int events ;
	int tb_start, tb_stop ;	// timebin window

//	struct tpx_odd_fee_t tpx_odd_fee[256] ;
//	int tpx_odd_fee_count  ;


	//int charge_peak ;	// peak of the charge
} ;

#endif
