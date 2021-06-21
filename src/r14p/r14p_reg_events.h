#ifndef R14P_REG_EVENTS_H_
#define R14P_REG_EVENTS_H_

#include <r14p.h>

namespace r14p {

	// fwd declaration
	class RegClientStreamDone;


	// Client registration stream done
	class RegClientStreamNew: public R14PCallbackMethod {
	public:
		RegClientStreamNew(R14PClient* _client);
		~RegClientStreamNew();

	        // event handler method
	        void run(r14p::R14PCallbackArgs* args);

	        // members
		uint32_t pm_dtype;
		uint32_t pm_did;
		uint32_t pm_router;
		uint32_t reg_action;
		int router_flag;
		R14PClient* client;
		R14PCallbackMethod* sdone;
	        // signal
	        //sem_t signal;
	        pmink::Atomic<uint8_t> done_signal;
	        // status
	        int status;


	};

	// Client registration stream next
	class RegClientStreamDone: public R14PCallbackMethod {
	public:
	        // event handler method
	        void run(r14p::R14PCallbackArgs* args);

	        // members
	        RegClientStreamNew* snew;
	};

}

#endif /* R14P_REG_EVENTS_H_ */
