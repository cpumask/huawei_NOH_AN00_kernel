#ifndef __QIC_INTERFACE_H__
#define __QIC_INTERFACE_H__ 
enum QIC_CORE_ID {
 QIC_ISP = 0x0,
 QIC_DSS,
 QIC_VENC,
 QIC_VDEC,
 QIC_IVP0,
 QIC_IVP1,
 QIC_IPP,
 QIC_ARPP,
 QIC_EPS,
 QIC_CORE_ID_MAX,
};
void hisi_qic_modid_register(unsigned int qic_coreid, unsigned int modid);
#endif
