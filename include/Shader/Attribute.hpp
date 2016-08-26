/*
 * ShaderAttribute.hpp
 *
 *  Created on: Jul 13, 2016
 *      Author: tyler
 */

#ifndef MyUPlay_MyEngine_Attribute
#define MyUPlay_MyEngine_Attribute

#include <array>
#include <functional>

#include "Shader/ShaderUtil.hpp"
#include "Shader/ShaderNode.hpp"

namespace MyUPlay{
	namespace MyEngine {
		namespace Shader {

			class IAttribute : IShaderNode {
			protected:
				IAttribute(std::string name) : name(name) {}
			public:
				const std::string name;

				virtual ~IAttribute(){}

				std::string getInstance() const override {
					return name;
				}
				virtual void push() = 0; //Pushes the native values to the gpu
			};

			/**
			 * Attribute is a class meant to take c variables and push them to the gpu.
			 * This class is heavily templated to allow any shader language to use it but
			 * then it must have the static variables/methods created for that renderer.
			 * Note: Copying this is a shallow copy. You should create a new instance
			 * with its own static array/data for a deep copy.
			 *
			 * Template:
			 * Renderer followed by the type the class holds.
			 * This will be automatically converted thanks to specializations in each render class
			 * Size: We put size in the template to allow some quick optimization and prevent users
			 * from changing attribute sizes over time. If you want to change "size" then create a
			 * max size one and a second that indicates its length. Otherwise the shader needs to
			 * be recompiled every time the size changes. If you don't want to use an array then
			 * leave this out and the class will specialize.
			 *
			 * @param name - Must be unique to the shader, don't use var### because it is the
			 * procedurally generated form that the nodes use.
			 * @param value - A shared pointer to a statically sized array or value.
			 */
			template <class R, typename T, unsigned size>
			class Attribute : IAttribute {
			private:
				std::shared_ptr<std::array<T, size>> value; //Always will have at least one element.
			public:

				static const char* type;
				Output<T> out;

				Attribute(const std::string& name, Scope s, std::shared_ptr<std::array<T, size>> value) : IAttribute(name, s), value(value) {}
				Attribute(std::string&& name, Scope s, std::shared_ptr<std::array<T,size>> value) : IAttribute(name, s), value(value) {}
				~Attribute(){}

				Attribute(const Attribute& a){ //Copy
					copy(a);
				}

				Attribute(Attribute&& a){ //Move
					value = a.value;
					name = a.name;
				}

				Attribute& copy(const Attribute& a){
					value = a.value;
					name = a.name;
					return *this;
				}

				Attribute& operator=(const Attribute& a){
					return copy(a);
				}

				//This must be defined with a partial specialization in each renderer R
				std::string getStatic() const override;
				void push() override;

			};

			template <class R, typename T>
			class Attribute : IAttribute {
			private:
				std::shared_ptr<T> value;
			public:

				static const char* type;

				Attribute(const std::string& name, Scope s, std::shared_ptr<T> value) : IAttribute(name, s), value(value) {}
				Attribute(std::string&& name, Scope s, std::shared_ptr<T> value) : IAttribute(name, s), value(value) {}
				~Attribute(){}

				Attribute(const Attribute& a){ //Copy
					copy(a);
				}

				Attribute(Attribute&& a){ //Move
					value = a.value;
					name = a.name;
				}

				Attribute& copy(const Attribute& a){
					value = a.value;
					name = a.name;
					return *this;
				}

				Attribute& operator=(const Attribute& a){
					return copy(a);
				}

				//This must be defined with a partial specialization in each renderer R
				std::string getStatic() const override;
				void push() override;

			};

			template <class R, typename T, unsigned size>
			const char* Attribute<R, T, size>::type = Utility<R, T>::Type;

			template <class R, typename T>
			const char* Attribute<R, T>::type = Utiltity<R, T>::Type;

			template <class T, unsigned size>
			std::string Attribute<GLES2Renderer, T, size>::getStatic() const {
				std::string ret;
				switch(scope){
					case Scope::PerFrame:
					ret = "uniform";
					break;
					case Scope::PerPrimative:
					ret = "attribute";
					break;
					case Scope::PerVertex:
					ret = "varying";
					break;
				}
				ret += " " + Attribute::type " " + name + "[" + size + "];";
				return ret;
			}

			template <class T>
			std::string Attribute<GLES2Renderer, T>::getStatic() const {
				std::string ret;
				switch(scope){
					case PerFrame:
					ret = "uniform";
					break;
					case PerPrimative:
					ret = "attribute";
					break;
					case PerVertex:
					ret = "varying";
					break;
				}
				ret += " " + Attribute::type " " + name + ";";
				return ret;
			}

		}
	}
}


#endif /* INCLUDE_SHADER_SHADERATTRIBUTE_HPP_ */
