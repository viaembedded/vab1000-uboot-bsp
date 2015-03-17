#include "scm_handle.h"

#define CACHELINESIZE 32

#define __asmeq(x, y)  ".ifnc " x "," y " ; .err ; .endif\n\t"

#ifndef offsetof
# define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif


/*alloc_scm_command() - Allocate an SCM command
 * @cmd_size: size of the command buffer
 * @resp_size: size of the response buffer
 *
 * Allocate an SCM command, including enough room for the command
 * and response headers as well as the command and response buffers.
 *
 * Returns a valid &scm_command on success or %NULL if the allocation fails.
 */

static struct scm_command *alloc_scm_command(size_t cmd_size, size_t resp_size)
{
	struct scm_command *cmd;
	struct scm_response *rsp;
	size_t len = sizeof(*cmd) + sizeof(*rsp) + cmd_size +
		resp_size;

	//len = (len + CACHELINESIZE - 1) & (~(CACHELINESIZE -1));

	cmd = malloc(len);
	if (cmd) {
		cmd->len = len;
		cmd->buf_offset = offsetof(struct scm_command, buf);
		cmd->resp_hdr_offset = cmd->buf_offset + cmd_size;
		
		rsp = (struct scm_response*)((void*)cmd + cmd->resp_hdr_offset);
		rsp->len = resp_size;
		rsp->buf_offset = sizeof(*rsp);
	}
	return cmd;
}

/**
 * free_scm_command() - Free an SCM command
 * @cmd: command to free
 *
 * Free an SCM command.
 */
static inline void free_scm_command(struct scm_command *cmd)
{
	/* we can't really free here, because u-boot cache is disabled, but secure os is cache enable */
	/* if pass the memory address, secure os can't get correct value */
	//free((void*)cmd);
}

/**
 * scm_command_to_response() - Get a pointer to a scm_response
 * @cmd: command
 *
 * Returns a pointer to a response for a command.
 */
static inline struct scm_response *scm_command_to_response(
		const struct scm_command *cmd)
{
	return (void *)cmd + cmd->resp_hdr_offset;
}

/**
 * scm_get_command_buffer() - Get a pointer to a command buffer
 * @cmd: command
 *
 * Returns a pointer to the command buffer of a command.
 */

static inline void *scm_get_command_buffer(const struct scm_command *cmd)
{
	return (void *)cmd->buf;
}

/**
 * scm_get_response_buffer() - Get a pointer to a response buffer
 * @rsp: response
 *
 * Returns a pointer to a response buffer of a response.
 */
static inline void *scm_get_response_buffer(const struct scm_response *rsp)
{
	return (void *)rsp + rsp->buf_offset;
}

static int scm_remap_error(int err)
{
	return 0;
}

static u32 smc(u32 cmd_addr)
{
	int context_id;
	register u32 r0 asm("r0") = 0;
	register u32 r1 asm("r1") = (u32)&context_id;
	register u32 r2 asm("r2") = cmd_addr;

	do {
		asm volatile(
			__asmeq("%0", "r0")
			__asmeq("%1", "r0")
			__asmeq("%2", "r1")
			__asmeq("%3", "r2")
			"push   {lr} \r\n"
			"mrs lr, spsr \r\n"
			"push   {lr} \r\n"
			"smc	#0	@ switch to secure world\r\n"
			"pop    {lr} \r\n"
			"msr spsr, lr \r\n"
			"pop    {lr}"
			: "=r" (r0)
			: "r" (r0), "r" (r1), "r" (r2)
			: "r3");
	} while(r0 == SCM_INTERRUPTED);

	return r0;
}

static int __scm_call(const struct scm_command *cmd)
{
	int ret;
	u32 cmd_addr = ((unsigned int)cmd);

	/*
	 * Flush the entire cache here so callers don't have to remember
	 * to flush the cache when passing physical addresses to the secure
	 * side in the buffer.
	 */
	ret = smc(cmd_addr);

	if (ret < 0)
		ret = scm_remap_error(ret);

	return ret;
}

/**
 * scm_call() - Send an SCM command
 * @svc_id: service identifier
 * @cmd_id: command identifier
 * @cmd_buf: command buffer
 * @cmd_len: length of the command buffer
 * @resp_buf: response buffer
 * @resp_len: length of the response buffer
 *
 * Sends a command to the SCM and waits for the command to finish processing.
 */
int scm_call(u32 svc_id, u32 cmd_id, const void *cmd_buf, size_t cmd_len,
		void *resp_buf, size_t resp_len)
{
	int ret;
	struct scm_command *cmd;
	struct scm_response *rsp;

	cmd = alloc_scm_command(cmd_len, resp_len);
	if (!cmd)
		return -1;
	cmd->id = (svc_id << 10) | cmd_id;
	if (cmd_buf)
		memcpy(scm_get_command_buffer(cmd), cmd_buf, cmd_len);

	ret = __scm_call(cmd);

	if (ret)
		goto out;

	if (!resp_buf)
		goto out;

	rsp = scm_command_to_response(cmd);

	if (resp_buf)
		memcpy(resp_buf, scm_get_response_buffer(rsp), resp_len);
out:
	free_scm_command(cmd);

	return ret;
}

int verify_image_by_secos(unsigned char *signature, unsigned char *src, size_t size) 
{
	int ret;
	int result = 0;
	cmd_verify_image_t cmd;
	cmd.sig = (char *)signature;
	cmd.src = (char *)src;
	cmd.with_hash = 0;
	cmd.from_uboot = 1;
	
	cmd.size = size;
	
	ret = scm_call(SVC_SYSTEM, SVC_SYSTEM_VERIFY_IMAGE, &cmd, sizeof(cmd), &result, sizeof(int));
	if(ret)
		printf("%s: cmd failed\n", __func__);
	printf("%s: result: %d\n", __func__, result);
	return (result == 1) ? 0 : -1;
}


int verify_signature_by_secos(unsigned char *signature, unsigned char *hash) 
{
	int ret;
	int result = 0;
	cmd_verify_image_t cmd;
	cmd.sig  = (char *)signature;
	cmd.hash = (char *)hash;
	cmd.with_hash = 1;
	cmd.from_uboot = 1;
	
	ret = scm_call(SVC_SYSTEM, SVC_SYSTEM_VERIFY_IMAGE, &cmd, sizeof(cmd), &result, sizeof(int));
	if(ret)
		printf("%s: cmd failed\n", __func__);
	printf("%s: result: %d\n", __func__, result);
	return (result == 1) ? 0 : -1;
}

int svc_system_verify_image(char *sig, char *src, int *result)
{
	int ret;
	cmd_verify_image_t cmd;
	cmd.sig = sig;
	cmd.src = src;
	cmd.with_hash = 0;
	cmd.from_uboot = 1;
	
	ret = scm_call(SVC_SYSTEM, SVC_SYSTEM_VERIFY_IMAGE, &cmd, sizeof(cmd), result, sizeof(int));
	
	return ret; 
}


unsigned int svc_system_read_pmu_reg(unsigned int reg)
{
        cmd_prog_pmu_reg_t cmd;
        cmd.read = 1;
        cmd.reg = reg;
        scm_call(SVC_SYSTEM, SVC_SYSTEM_PROG_PMU_REG, &cmd, sizeof(cmd), &cmd.value, sizeof(unsigned int));
        return cmd.value;
}

void svc_system_write_pmu_reg(unsigned int reg, unsigned int value)
{
        cmd_prog_pmu_reg_t cmd;
        cmd.read = 0;
        cmd.reg = reg;
        cmd.value = value;
        scm_call(SVC_SYSTEM, SVC_SYSTEM_PROG_PMU_REG, &cmd, sizeof(cmd), 0, 0);
}

