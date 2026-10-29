 
 
#ifndef ENGINE_SHARED_COMPRESSION_H
#define ENGINE_SHARED_COMPRESSION_H


class CVariableInt
{
public:
	enum
	{
		MAX_BYTES_PACKED = 5, 
	};

	
	
	static unsigned char *Pack(unsigned char *pDst, int i, int DstSize)
	{
		if(DstSize <= 0)
			return nullptr;

		DstSize--;
		*pDst = 0;
		if(i < 0)
		{
			*pDst |= 0x40; 
			i = ~i;
		}

		*pDst |= i & 0x3F; 
		i >>= 6; 
		while(i)
		{
			if(DstSize <= 0)
				return nullptr;
			*pDst |= 0x80; 
			DstSize--;
			pDst++;
			*pDst = i & 0x7F; 
			i >>= 7; 
		}

		pDst++;
		return pDst;
	}

	static const unsigned char *Unpack(const unsigned char *pSrc, int *pInOut, int SrcSize);

	static long Compress(const void *pSrc, int SrcSize, void *pDst, int DstSize);
	static long Decompress(const void *pSrc, int SrcSize, void *pDst, int DstSize);
};

#endif
