
#ifndef USERDEFINE_H_
#define USERDEFINE_H_

#if 1
#define DEBUG_DEB(fmt, arg...) printf("[DEBUG][%s,%d]"fmt, __FUNCTION__, __LINE__, ##arg)
#else
#define DEBUG_DEB(fmt, arg...)
#endif

#if 0
#define DEBUG_INFO(fmt, arg...) printf("[INFO][%s,%d]"fmt, __FUNCTION__, __LINE__, ##arg)
#else
#define DEBUG_INFO(fmt, arg...)
#endif

#if 1
#define DEBUG_ERR(fmt, arg...) printf("[ERROR][%s,%d]"fmt, __FUNCTION__, __LINE__, ##arg)
#else
#define DEBUG_ERR(fmt, arg...)
#endif

typedef enum
{
	FALSE,
	TRUE,
}BOOL;

typedef enum
{
	test,
	t,
	y,
}TEST;
#endif /* USERDEFINE_H_ */
