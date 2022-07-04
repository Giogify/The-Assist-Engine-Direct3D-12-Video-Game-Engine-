/*
/  ----------------------------------------------
/  Keyboard.h
/  
/  A data system that handles the keyboard 
/  KeyDown and KeyUp events as well as storing 
/  these events in a queue for interpreting 
/  multiple keys / keys at a later time.
/  
/  Also has functionality for reading and storing 
/  input characters from WM_CHAR events.
/  ----------------------------------------------
*/
#pragma once
#include <queue>
#include <bitset>

namespace GID {
	namespace DSU {
		struct Keyboard {

			// Event Class
			struct Event {

				// Type of Event
				enum class Type {
					Press,
					Release,
					Invalid
				};

				// Fields
				Type			type = {};
				uint8_t			code = {};

				// Constructors
				Event() noexcept :
					type(Type::Invalid),
					code(0u) {}

				Event(Type type, uint8_t code) noexcept :
					type(type),
					code(code) {}

				// Getters/Setters/Is
				uint8_t getCode() const noexcept { return code; }
				bool isPress() const noexcept { return type == Type::Press; }
				bool isRelease() const noexcept { return type == Type::Release; }
				bool isValid() const noexcept { return type != Type::Invalid; }

			};

			// Fields
			static constexpr uint16_t		MKEYCOUNT{ 256u };
			static constexpr uint8_t		MBUFFERSIZE{ 16u };
			bool							mAutoRepeatEnabled{ false };
			std::bitset<MKEYCOUNT>			mKeyStates{};
			std::queue<Event>				mKeyBuffer{};
			std::queue<int8_t>				mCharBuffer{};

			static void trimBuffer(std::queue<Event>& buffer) noexcept {
				while (buffer.size() > MBUFFERSIZE) {
					buffer.pop();
				}
			}

			static void trimBuffer(std::queue<int8_t>& buffer) noexcept {
				while (buffer.size() > MBUFFERSIZE) {
					buffer.pop();
				}
			}

			// Part of the Window
			void onKeyPressed(uint8_t keycode) noexcept {
				mKeyStates[keycode] = true;
				mKeyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
				trimBuffer(mKeyBuffer);
			}
			void onKeyReleased(uint8_t keycode) noexcept {
				mKeyStates[keycode] = false;
				mKeyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
				trimBuffer(mKeyBuffer);
			}
			void onChar(int8_t character) noexcept {
				mCharBuffer.push(character);
				trimBuffer(mCharBuffer);
			}
			void clearState() noexcept {
				mKeyStates.reset();
			}

			// Constructors
			Keyboard() = default;

			// Key Event
			bool isKeyPressed(uint8_t keycode) const noexcept {
				return mKeyStates[keycode];
			}
			Event readKey() noexcept {
				if (mKeyBuffer.size() > 0u) {
					Keyboard::Event e = mKeyBuffer.front();
					mKeyBuffer.pop();
					return e;
				}
				else { return Keyboard::Event(); }
			}
			bool isKeyQueueEmpty() const noexcept {
				return mKeyBuffer.empty();
			}
			void clearKeyQueue() noexcept {
				mKeyBuffer = std::queue<Event>();
			}

			// Char Event
			int8_t readChar() noexcept {
				if (mCharBuffer.size() > 0u) {
					uint8_t charCode = mCharBuffer.front();
					mCharBuffer.pop();
					return charCode;
				}
				else return 0;
			}
			bool isCharQueueEmpty() const noexcept {
				return mCharBuffer.empty();
			}
			void clearCharQueue() noexcept {
				mCharBuffer = std::queue<int8_t>();
			}

			// Key and Char Event
			void clear() noexcept {
				clearKeyQueue();
				clearCharQueue();
			}

			// Auto-Repeat Controls
			void enableAutoRepeat() noexcept {
				mAutoRepeatEnabled = true;
			}
			void disableAutoRepeat() noexcept {
				mAutoRepeatEnabled = false;
			}
			bool isAutoRepeatEnabled() const noexcept {
				return mAutoRepeatEnabled;
			}

			// Operator Overloads
			//Keyboard& operator=(const Keyboard& kb) = delete;

		};
	}
}