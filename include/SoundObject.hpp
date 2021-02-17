#pragma once

#include <string>

#include "Vector3.hpp"


namespace MyEngine {

	class Listener {

		Listener();

		static Listener* singleton;

	public:

		static Listener* getInstance() {
			return singleton;
		}

		//A listener is expected to have a location, velocity, and direction.
		/**
		 * This enables hasVelocity and sets the velocity value.
		 *
		 * Velocity has no unit, so just keep it relative to other values in the
		 * game. (See openal)
		 */
		Listener& setVelocity(float);

		/**
		 * This sets the location/source of the sound. (Global position)
		 */
		Listener& setPosition(const Vector3<float>&);

		/**
		 * Sets the direction the object is casting the sound. (Global rotation normal)
		 */
		Listener& setDirection(const Vector3<float>&);


	};

	struct SoundObject {

		bool repeat = false;

		inline bool& getRepeat() {
			return repeat;
		}

		inline void setRepeat(bool val) {
			repeat = val;
		}


		/**
		 * This function starts playing the audio, if this
		 * particular node is already active then it will
		 * restart the sound, nodes of the same file data
		 * will be unaffected.
		 *
		 * If repeat is set to true when the sound ends, it
		 * will automatically start playing again.
		 */
		SoundObject& play();

		/**
		 * This function is expected to set this objects audio track, it
		 * also is expected to cache sounds its seen before and reuse their
		 * data instead of loading from disk every time (but only if the file
		 * name is exactly the same).
		 *
		 * This function can throw a runtime_error if the file format is unsupported.
		 */
		SoundObject& setAudio(std::string filename);

		/**
		 * @return true if the audio is already playing.
		 */
		bool isPlaying() const;

		/**
		 * @return the length in seconds of the audio.
		 */
		float getLength() const;

		/**
		 * This enables hasVelocity and sets the velocity value.
		 *
		 * Velocity has no unit, so just keep it relative to other values in the
		 * game. (See openal)
		 */
		SoundObject& setVelocity(float);

		/**
		 * This sets the location/source of the sound. (Global position)
		 */
		SoundObject& setPosition(const Vector3<float>&);

		/**
		 * Sets the direction the object is casting the sound. (Global rotation normal)
		 */
		SoundObject& setDirection(const Vector3<float>&);

	};

}
