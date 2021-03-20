/*
 * teek_client_list.h
 *
 * define exported funcs for list actions
 *
 * Copyright (c) 2012-2019 Huawei Technologies Co., Ltd.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#include <linux/types.h>

#ifndef _TEE_CLIENT_LIST_H_
#define _TEE_CLIENT_LIST_H_
/*
 * @ingroup TEEC_List
 * �������Ͷ���
 */
struct list_node {
	struct list_node *next;
	/* < ָ��next�ڵ�   */
	struct list_node *prev;
	/* < ָ��prev�ڵ�   */
};

/*
 * @ingroup TEEC_List
 * @brief ����һ������ڵ�
 * @par ����:
 * �궨�壬����һ������ڵ㣬�������ʼ��
 * @param name [IN] �������������
 */
#define LIST_DECLARE(name) \
	struct list_node (name) = { \
		.next = &(name), \
		.prev = &(name), \
	}

#ifndef NULL
#define NULL 0
#endif

/*
 * @ingroup TEEC_List
 * ��ȡ����list��prev�ڵ�
 */
#define LIST_TAIL(list) ((list)->prev)

/*
 * @ingroup TEEC_List
 * �ж�����list�Ƿ�Ϊ��
 */
#define LIST_EMPTY(list) ((list) == (list)->next)

/*
 * @ingroup  TEEC_List
 * @brief ������ͷ�������½ڵ�
 *
 * @par ����:
 * ������ͷ�������½ڵ�
 *
 * @attention ��
 * @param list [IN/OUT]����ͷָ�룬ȡֵ����Ϊ��
 * @param entry [IN/OUT]��������ڵ�ָ�룬ȡֵ����Ϊ��
 *
 * @retval ��
 *
 * @par ����:
 * @li libteec���ýӿ������Ĺ����
 * @li tee_client_list.h���ýӿ���������ͷ�ļ�
 * @see list_insert_tail
 * @since V100R002C00B301
 */
static inline void list_insert_head(struct list_node *list,
	struct list_node *entry)
{
	list->next->prev = entry;
	entry->next = list->next;
	entry->prev = list;
	list->next = entry;
}

/*
 * @ingroup  TEEC_List
 * @brief ������β�������½ڵ�
 *
 * @par ����:
 * ������β�������½ڵ�
 *
 * @attention ��
 * @param list [IN/OUT]����ͷָ�룬ȡֵ����Ϊ��
 * @param entry [IN/OUT]��������ڵ�ָ�룬ȡֵ����Ϊ��
 *
 * @retval ��
 *
 * @par ����:
 * @li libteec���ýӿ������Ĺ����
 * @li tee_client_list.h���ýӿ���������ͷ�ļ�
 * @see list_insert_head
 * @since V100R002C00B301
 */
static inline void list_insert_tail(struct list_node *list,
	struct list_node *entry)
{
	entry->next = list;
	entry->prev = list->prev;
	list->prev->next = entry;
	list->prev = entry;
}

/*
 * @ingroup  TEEC_List
 * @brief ɾ���ڵ�
 *
 * @par ����:
 * ɾ��ָ���Ľڵ�
 *
 * @attention ���غ��û���Ҫ�ͷ�ɾ���ڵ���ڴ�
 * @param entry [IN]��ɾ��������ڵ�ָ�룬ȡֵ����Ϊ��
 *
 * @retval ��
 *
 * @par ����:
 * @li libteec���ýӿ������Ĺ����
 * @li tee_client_list.h���ýӿ���������ͷ�ļ�
 * @see ��
 * @since V100R002C00B301
 */
static inline void list_remove(struct list_node *entry)
{
	entry->prev->next = entry->next;
	entry->next->prev = entry->prev;
}

/*
 * @ingroup  TEEC_List
 * @brief ɾ������ͷ���
 *
 * @par ����:
 * ɾ��ָ�������ͷ�ڵ�
 *
 * @attention ���غ��û������ͷ�ɾ���ڵ���ڴ�
 * @param list [IN]����ͷָ�룬ȡֵ����Ϊ��
 *
 * @retval #NULL ����Ϊ��
 * @retval ��NULL ����ͷ���
 *
 * @par ����:
 * @li libteec���ýӿ������Ĺ����
 * @li tee_client_list.h���ýӿ���������ͷ�ļ�
 * @see list_remove_tail
 * @since V100R002C00B301
 */
static inline struct list_node *list_remove_head(struct list_node *list)
{
	struct list_node *entry = NULL;
	if (!LIST_EMPTY(list)) {
		entry = list->next;
		list_remove(entry);
	}
	return entry;
}

/*
 * @ingroup  TEEC_List
 * @brief ɾ������β���
 *
 * @par ����:
 * ɾ��ָ�������β�ڵ�
 *
 * @attention ���غ��û������ͷ�ɾ���ڵ���ڴ�
 * @param list [IN]����ͷָ�룬ȡֵ����Ϊ��
 *
 * @retval NULL ����Ϊ��
 * @retval �ǿ� ����β���
 *
 * @par ����:
 * @li libteec���ýӿ������Ĺ����
 * @li tee_client_list.h���ýӿ���������ͷ�ļ�
 * @see list_remove_head
 * @since V100R002C00B301
 */
static inline struct list_node *list_remove_tail(struct list_node *list)
{
	struct list_node *entry = NULL;
	if (!LIST_EMPTY(list)) {
		entry = list->prev;
		list_remove(entry);
	}
	return entry;
}
#endif
