#ifndef hashers_h__
#define hashers_h__


namespace hashers
{

//http://burtleburtle.net/bob/hash/integer.html
struct int_hash_1
{
	size_t
		operator()(uint32_t a) const
	{
		a = (a+0x7ed55d16) + (a<<12);
		a = (a^0xc761c23c) ^ (a>>19);
		a = (a+0x165667b1) + (a<<5);
		a = (a+0xd3a2646c) ^ (a<<9);
		a = (a+0xfd7046c5) + (a<<3);
		a = (a^0xb55a4f09) ^ (a>>16);

		return a; }
};

struct int_hash_2
{
	size_t
		operator()(uint32_t a) const
	{
		a = a ^ (a>>4);
		a = (a^0xdeadbeef) + (a<<5);
		a = a ^ (a>>11);	
		return a; }
};

struct int_hash_3
{
	size_t
		operator()(uint32_t a) const
	{
		a += ~(a<<15);
		a ^=  (a>>10);
		a +=  (a<<3);
		a ^=  (a>>6);
		a += ~(a<<11);
		a ^=  (a>>16);
		return a; }
};

}


#endif // hashers_h__
