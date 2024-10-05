#ifndef INC_DYNAMIXEL_COMMAND_QUEUE_H_
#define INC_DYNAMIXEL_COMMAND_QUEUE_H_


#include <cmath>
#include <functional>
#include <iostream>
#include <cstring>

#include "hal_callbacks.h"
#include "usart.h"

#include "cpp_circular_buffer.h"



using std::placeholders::_1;


class DynamixelCommandQueue {
public:
	explicit DynamixelCommandQueue(UART_HandleTypeDef* huart): m_huart{huart}, m_isAwaitResponse{false} {
		std::function<void(uint16_t)> callbackFunction =
		      std::bind(&DynamixelCommandQueue::processReadData, this, _1);
		  halCallbacks_registerUartRxIdleCallback(m_huart, callbackFunction);

	}

	uint8_t sendMessage(uint8_t* buffer, uint16_t writeLength, std::function<void(uint16_t)> callback, uint8_t *callbackBuffer) {
		if (m_messageBuffer.full()) {
			return 1;
		}
		else {
			CommandData newCommand;
			std::memcpy(&(newCommand.message), buffer, writeLength);
			newCommand.messageSize = writeLength;
			newCommand.callback = callback;
			newCommand.responseBuffer = callbackBuffer;
			m_messageBuffer.enqueue(newCommand);
			return 0;
		}
	}

	void processReadData(uint16_t size) {
		if (m_currentMessageCallback) {
			m_currentMessageCallback(size);
		}
	}


	void tick() {
		if (m_messageBuffer.count() == 0) {
			return;
		}
		if (m_isAwaitResponse && (HAL_GetTick() - m_prevSendTimeMs < kResponseTimeoutMs)) {
			return;
		}
		CommandData currentCommand;
		m_messageBuffer.peek(&currentCommand, 1);
		m_messageBuffer.dequeue(1);
		m_currentMessageCallback = currentCommand.callback;
		HAL_HalfDuplex_EnableTransmitter(m_huart);
		HAL_UART_Transmit(m_huart, (uint8_t*)&(currentCommand.message), currentCommand.messageSize, 100);
		HAL_Delay(200);
		m_prevSendTimeMs = HAL_GetTick();
		m_isAwaitResponse = true;
		HAL_HalfDuplex_EnableReceiver(m_huart);
		HAL_UARTEx_ReceiveToIdle_IT(m_huart, currentCommand.responseBuffer, kMaxBufferSize);

	}

private:

	static const constexpr uint32_t kMaxBufferSize = 100;
	static const constexpr uint32_t kMaxMessageCount = 100;
	static const constexpr uint32_t kResponseTimeoutMs = 5000;
	struct CommandData {
		  uint8_t message[kMaxBufferSize];
		  uint16_t messageSize;
		  uint8_t *responseBuffer;
		  std::function<void(uint16_t)> callback;
	};
	CircularBuffer<CommandData, 100> m_messageBuffer;
	UART_HandleTypeDef* m_huart;
	std::function<void(uint16_t)> m_currentMessageCallback;
	bool m_isAwaitResponse;
	uint32_t m_prevSendTimeMs;



};



#endif /* INC_DYNAMIXEL_COMMAND_QUEUE_H_ */
