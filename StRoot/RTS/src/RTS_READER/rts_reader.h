#ifndef _RTS_READER_H_
#define _RTS_READER_H_

/*!
                                                                     
\class  rts_reader
\author tonko
\date   2007/10/27
\brief  rts_reader is an attempt to get some DAQ reader classes working

This is where I type some longer comment...
Not sure if this shows up anywhere but I'll check with Yuri.	

*/                                                                      

#include <sys/types.h>	// needed for u_int etc.


// forward decls...
class sfs_index ;
class fs_dir ;
class daq_det ;


class rts_reader {
private:
	int sfs_owner ;

	int get_event() ;
	int handle_open() ;
	void handle_close() ;

	int handle_legacy() ;
	int find_leaf(const char *top, const char *leaf, char *result) ;

	int m_Mode ;
public:
	rts_reader(const char *name="rts_rdr") ;	// Offline wants char stuff...
	virtual ~rts_reader() ;

	virtual int Init() ;

	virtual int InitRun(int run) ;
	virtual int Make() ;	// Offline; advances to next event!
	virtual int FinishRun(int old) ;



	virtual void SetMode(int mode=0) { m_Mode=mode ;} ;
	virtual int GetMode() { return m_Mode ; } ;
	



	virtual const char *GetCVS() const {	// Offline
		static const char cvs[]="Tag $Name:  $Id: built "__DATE__" "__TIME__ ; return cvs;
	}

	// ******************* here we can put our own stuff
	int add_input(const char *filename) ;		// we call this to import a list of filenames
	int add_input(class sfs_index *usesfs) ;			// do not open new files

	int enable(const char *dets="*") ;		// takes a space separated list of detectors we are interested in; others are ignored
	int enable(u_int rts_mask) ;

	daq_det *det(const char *which="*") ;		// choose det by name i.e. "tpx" 
	daq_det *det(u_int rts_mask) ;			// chosse det by RTS MASK i.e. (1<<TPC_ID)

	// I might make some of this private but I want them now public for debugging

	daq_det *dispatcher ;			// this is a dummy det used for dispatching multi det calls

	int l_errno ;				// my local errno
	const char *iname ;

	u_int cur_run_number ;			// run number of the current event
	u_int cur_file_ix ;			// file index of the current file
	int cur_desc ;

	u_int cur_event_in_file ;		// index in the current file of the current event
	u_int cur_event ;			// index (aka count) of the current event over all file

	const char *select_files[1024] ;	// pointers to filenames
	u_int num_files ;			// how many...

	char dets_enabled[1024] ;	// as a string i.e. "tpx hft"
	u_int rts_dets_enabled ;	// as a RTS_ID bitmask


	sfs_index	*sfs ;					// "driver" for the new file format library
	fs_dir		*fs_root_dir ;			// determined in handle_open

	char		fs_cur_evt[128] ;			// name of the form "/#123123" or "/tpx", just that part!
	u_int		cur_evt_ix ;				// this is the number after /# of the current entry

	char		*legacy_p ;
	u_int		legacy_bytes ;

} ;


#endif
