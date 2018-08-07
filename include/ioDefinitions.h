/**
 * @author	K
 * @date		07-06-2017.
 */

#ifndef EMBEDDED_IOREGISTERS_H
#define EMBEDDED_IOREGISTERS_H

namespace velo
{
	namespace mcu
	{
		namespace io
		{
#define IO_REG_WRAPPER(REG_NAME, CLASS_NAME, DATA_TYPE)     	\
			struct CLASS_NAME{                                    \
				typedef DATA_TYPE data;                           	\
				static data get(){return REG_NAME;}               	\
				static void set(data value){REG_NAME = value;}    	\
				static void OR(data value){REG_NAME |= value;}    	\
				static void AND(data value){REG_NAME &= value;}   	\
				static void XOR(data value){REG_NAME ^= value;}   	\
				static void AOR(data andMask, data orMask)     		\
					{REG_NAME = (REG_NAME & andMask) | orMask;}     \
				template<int Bit>                                  \
				static bool bitIsSet()                             \
					{return REG_NAME & (1 << Bit);}                 \
				template<int Bit>                                  \
				static bool bitIsClear()                           \
					{return !(REG_NAME & (1 << Bit));}              \
			}

#define I_REG_WRAPPER(REG_NAME, CLASS_NAME, DATA_TYPE)   		\
			struct CLASS_NAME                                     \
			{                                                     \
				typedef DATA_TYPE data;                           	\
				static data get(){return REG_NAME;}               	\
			}

#define IO_STRUCT_WRAPPER(STRUCT_PTR, CLASS_NAME, STRUCT_TYPE)            		\
			struct CLASS_NAME                                                    \
			{                                                                    \
				typedef STRUCT_TYPE data;                                        	\
				STRUCT_TYPE* operator->(){return ((STRUCT_TYPE *)(STRUCT_PTR));}	\
			}

#define IO_BITFIELD_WRAPPER(REG_NAME, CLASS_NAME, DATA_TYPE, BITFIELD_OFFSET, BITFIELD_LENGTH)				\
			struct CLASS_NAME																											\
			{																																\
				typedef DATA_TYPE data;																								\
				static const data mask = ((data(1u) << BITFIELD_LENGTH) - 1);											\
				static data get(){return (REG_NAME >> BITFIELD_OFFSET) & mask;}										\
				static void set(data value)																						\
				{REG_NAME = (REG_NAME & ~(mask << BITFIELD_OFFSET)) | ((value & mask) << BITFIELD_OFFSET);}	\
			}
		}
	}
}

#endif //EMBEDDED_IOREGISTERS_H
