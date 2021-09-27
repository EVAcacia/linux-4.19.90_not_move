// SPDX-License-Identifier: GPL-2.0
/*
 * fs/sysfs/symlink.c - operations for initializing and mounting sysfs
 *
 * Copyright (c) 2001-3 Patrick Mochel
 * Copyright (c) 2007 SUSE Linux Products GmbH
 * Copyright (c) 2007 Tejun Heo <teheo@suse.de>
 *
 * Please see Documentation/filesystems/sysfs.txt for more information.
 */

#include <linux/fs.h>
#include <linux/magic.h>
#include <linux/mount.h>
#include <linux/init.h>
#include <linux/user_namespace.h>

#include "sysfs.h"

static struct kernfs_root *sysfs_root;
struct kernfs_node *sysfs_root_kn;

/**
 * lsh:创建一个新的层
*/
static struct kernfs_root *lshfs_root;
struct kernfs_node *lshfs_root_kn;

static struct dentry *sysfs_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
{
	struct dentry *root;
	void *ns;
	bool new_sb = false;

	if (!(flags & SB_KERNMOUNT)) {
		if (!kobj_ns_current_may_mount(KOBJ_NS_TYPE_NET))
			return ERR_PTR(-EPERM);
	}

	ns = kobj_ns_grab_current(KOBJ_NS_TYPE_NET);
	root = kernfs_mount_ns(fs_type, flags, sysfs_root,
				SYSFS_MAGIC, &new_sb, ns);
	if (!new_sb)
		kobj_ns_drop(KOBJ_NS_TYPE_NET, ns);
	else if (!IS_ERR(root))
		root->d_sb->s_iflags |= SB_I_USERNS_VISIBLE;

	return root;
}

//lsh
/**
 * lshfs_mount: .mount成员函数负责超级块、根目录和索引节点的创建和初始化工作
*/
static struct dentry *lshfs_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
{
	struct dentry *root;
	void *ns;
	bool new_sb = false;

	if (!(flags & SB_KERNMOUNT)) {
		if (!kobj_ns_current_may_mount(KOBJ_NS_TYPE_NET))
			return ERR_PTR(-EPERM);
	}

	ns = kobj_ns_grab_current(KOBJ_NS_TYPE_NET);
	root = kernfs_mount_ns(fs_type, flags, lshfs_root,
				SYSFS_MAGIC, &new_sb, ns);
	if (!new_sb)
		kobj_ns_drop(KOBJ_NS_TYPE_NET, ns);
	else if (!IS_ERR(root))
		root->d_sb->s_iflags |= SB_I_USERNS_VISIBLE;

	return root;
}

static void sysfs_kill_sb(struct super_block *sb)
{
	void *ns = (void *)kernfs_super_ns(sb);

	kernfs_kill_sb(sb);
	kobj_ns_drop(KOBJ_NS_TYPE_NET, ns);
}

static void lshfs_kill_sb(struct super_block *sb)
{
	void *ns = (void *)kernfs_super_ns(sb);

	kernfs_kill_sb(sb);
	kobj_ns_drop(KOBJ_NS_TYPE_NET, ns);
}

static struct file_system_type sysfs_fs_type = {
	.name		= "sysfs",
	.mount		= sysfs_mount,
	.kill_sb	= sysfs_kill_sb,
	.fs_flags	= FS_USERNS_MOUNT,
};

static struct file_system_type lshfs_fs_type = {
	.name		= "lshfs",
	.mount		= lshfs_mount,
	.kill_sb	= lshfs_kill_sb,
	.fs_flags	= FS_USERNS_MOUNT,
};


//注册并挂载sysfs文件系统，然后调用kobject_create_and_add()创建"fs"目录。
int __init sysfs_init(void)
{
	int err;
	
	/**
	 * kernfs层的api:
	 * kernfs 是 kernel 3.14 引入的内存文件系统。
	 * 相比先前的 sysfs，和 VFS 分离，并解决了死锁的老大难问题。
	 * 在 sysfs: separate out kernfs 一文中，Tejun 将 sysfs 的逻辑层和表示层分离。
	 * 其中，专门负责创建伪文件系统的表示层被单独拿出来作为 kernfs，
	 * 从而让其能作为独立组件供其他模块使用。
	*/
	sysfs_root = kernfs_create_root(NULL, KERNFS_ROOT_EXTRA_OPEN_PERM_CHECK,NULL);// 创建新的 kernfs 层级
	if (IS_ERR(sysfs_root))
		return PTR_ERR(sysfs_root);

	lshfs_root = kernfs_create_root(NULL, KERNFS_ROOT_EXTRA_OPEN_PERM_CHECK,NULL);// 创建新的 kernfs 层级
	if (IS_ERR(lshfs_root))
		return PTR_ERR(lshfs_root);

	sysfs_root_kn = sysfs_root->kn;
	lshfs_root_kn = lshfs_root->kn;

	err = register_filesystem(&sysfs_fs_type);
	if (err) {
		kernfs_destroy_root(sysfs_root);
		return err;
	}

	err = register_filesystem(&lshfs_fs_type);
	if (err) {
		kernfs_destroy_root(lshfs_root);
		return err;
	}

	return 0;
}
