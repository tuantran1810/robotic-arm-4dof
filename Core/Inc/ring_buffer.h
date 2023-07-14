/*
 * ring_buffer.h
 *
 *  Created on: Jul 12, 2023
 *      Author: tuantran
 */

#ifndef INC_RING_BUFFER_H_
#define INC_RING_BUFFER_H_


struct RingBuffer256 {
	uint8_t buffer[256];
	uint8_t start;
	uint8_t end;

	RingBuffer256() {
		this->start = 0;
		this->end = 0;
	}

	bool push(uint8_t* data, uint8_t length) {
		if (data == NULL) {
			return false;
		}
		uint8_t new_end = end + length;
		if ((this->start <= this->end) && (new_end > this->start) && (new_end <= this->end)) {
			return false;
		}
		if ((this->start > this->end) && ((new_end > this->start) || (new_end <= this->end))) {
			return false;
		}
		for (uint8_t i = 0; i < length; i++) {
			this->buffer[this->end] = data[i];
			this->end++;
		}
		return true;
	}

	uint8_t pop(void) {
		if (this->start == this->end) {
			return 0;
		}
		uint8_t c = this->buffer[this->start];
		this->start++;
		return c;
	}
};


#endif /* INC_RING_BUFFER_H_ */
