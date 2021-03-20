/*
 * Copyright (c) Hisilicon Technologies Co., Ltd. 2019-2019. All rights reserved.
 * Description: contexthub fmk header file.
 * Author: Huawei
 * Create: 2019/11/25
 */
#ifndef __INPUTHUB_WRAPPER_H__
#define __INPUTHUB_WRAPPER_H__
#include <linux/hisi/contexthub/protocol_as.h>
#include <linux/hisi/contexthub/iomcu_ipc.h>

/* inputhub_wrapper_pack_and_send_cmd max data len
 * contains only the data length, excluding the header information and subcommands
 */
#define INPUTHUB_WRAPPER_MAX_LEN (MAX_PKT_LENGTH - sizeof(pkt_header_t) - sizeof(u32))

/*
 * Function    : inputhub_wrapper_register_event_notifier
 * Description : register_event_notifier wrapper, to make protocol transparent to users
 * Input       : [tag] match tag
 *             : [cmd] match cmd
 *             : [notify] notify callback impl
 * Output      : none
 * Return      : 0:successs, others: fail
 */
int inputhub_wrapper_register_event_notifier(int tag, int cmd,
				int (*notify)(const pkt_header_t *head));

/*
 * Function    : inputhub_wrapper_unregister_event_notifier
 * Description : unregister_event_notifier wrapper, to make protocol transparent to users
 * Input       : [tag] match tag
 *.............: [cmd] match cmd
 *             : [notify] notify callback impl
 * Output      : none
 * Return      : 0:successs, others: fail
 */
int inputhub_wrapper_unregister_event_notifier(int tag, int cmd,
				int (*notify)(const pkt_header_t *head));

/*
 * Function    : inputhub_wrapper_send_cmd
 * Description : send cmd to sensorhub
 * Input       : [app_tag] app tag
 *             : [cmd] cmd
 *             : [buf] write info buffer
 *             : [buf_size] write info size
 *             : [rd] response info buffer
 * Output      : none
 * Return      : 0:successs, others: fail
 */
int inputhub_wrapper_send_cmd(int app_tag, int cmd, const void *buf, int buf_size,
			  struct read_info *rd);
/*
 * Function    : inputhub_wrapper_get_app_tag
 * Description : get app tag for head
 * Input       : [head] pakcet header
 * Output      : none
 * Return      : 0:successs, others: fail
 */
int inputhub_wrapper_get_app_tag(const pkt_header_t *head);

/*
 * Function    : inputhub_wrapper_pack_and_send_cmd
 * Description : send config/open/close cmd to sensorhub
 * Input       : [app_tag] app tag
 *             : [cmd] cmd
 *             : [sub_cmd] sub cmd
 *             : [buf] write info buffer
 *             : [count] write info size
 *             : [rd_info] response info buffer
 * Output      : none
 * Return      : 0:successs, others: fail
 */
int inputhub_wrapper_pack_and_send_cmd(int app_tag, int cmd, int sub_cmd, const char *buf, size_t count,
					struct read_info *rd_info);

#ifdef CONFIG_INPUTHUB_30
/*
 * Function    : inputhub_wrapper_register_event_notifier_ex
 * Description : register_event_notifier_ex
 * Input       : [match] match func
 *             : [notify] notify callback impl
 * Output      : none
 * Return      : 0:successs, others: fail
 */
int inputhub_wrapper_register_event_notifier_ex(
				int (*match)(const pkt_header_t *head),
				int (*notify)(const pkt_header_t *head));


/*
 * Function    : inputhub_wrapper_unregister_event_notifier_ex
 * Description : unregister_event_notifier_ex
 * Input       : [match] match func
 *             : [notify] notify callback impl
 * Output      : none
 * Return      : 0:successs, others: fail
 */
int inputhub_wrapper_unregister_event_notifier_ex(
				int (*match)(const pkt_header_t *head),
				int (*notify)(const pkt_header_t *head));
#endif

#endif
