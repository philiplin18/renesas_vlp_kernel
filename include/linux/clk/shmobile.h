/*
 * Copyright 2013 Ideas On Board SPRL
 * Copyright 2013, 2014 Horms Solutions Ltd.
 *
 * Contact: Laurent Pinchart <laurent.pinchart@ideasonboard.com>
 * Contact: Simon Horman <horms@verge.net.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __LINUX_CLK_SHMOBILE_H_
#define __LINUX_CLK_SHMOBILE_H_

#include <linux/types.h>

struct device;
struct device_node;
struct generic_pm_domain;

void r8a7778_clocks_init(u32 mode);
void r8a7779_clocks_init(u32 mode);
void rcar_gen2_clocks_init(u32 mode);

#ifdef CONFIG_PM_GENERIC_DOMAINS_OF
void cpg_mstp_add_clk_domain(struct device_node *np);
int cpg_mstp_attach_dev(struct generic_pm_domain *domain, struct device *dev);
void cpg_mstp_detach_dev(struct generic_pm_domain *domain, struct device *dev);
#else
static inline void cpg_mstp_add_clk_domain(struct device_node *np) {}
#endif

#define cpg_mssr_attach_dev	NULL
#define cpg_mssr_detach_dev	NULL

#endif
