// Copyright (c) 2015, Galaxy Authors. All Rights Reserved
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// Author: wangtaize@baidu.com

#include "agent/workspace.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sstream>
#include <unistd.h>
#include "common/logging.h"

namespace galaxy {

int DefaultWorkspace::Create() {
    LOG(INFO, "create workspace for task %d", m_task_info.task_id());
    if (m_has_created) {
        return 0;
    }
    //TODO safe path join
    std::stringstream ss;
    ss << m_root_path << "/" << m_task_info.task_id();
    m_task_root_path = ss.str();
    int status ;
    status = mkdir(m_task_root_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (status == 0) {
        m_has_created = true;
    }
    return status;
}

std::string DefaultWorkspace::GetPath() {
    return m_task_root_path;
}

int DefaultWorkspace::Clean() {
    LOG(INFO,"clean task %d workspace",m_task_info.task_id());
    if (!m_has_created) {
        return 0;
    }
    struct stat st;
    int ret = 0;
    if (stat(m_task_root_path.c_str(), &st) == 0 && S_ISDIR(st.st_mode)) {
        std::string rm_cmd ;
        rm_cmd = "rm -rf " + m_task_root_path;
        ret = system(rm_cmd.c_str());
        if (ret == 0) {
            LOG(INFO,"clean task %d workspace successfully",m_task_info.task_id());
            m_has_created = false;
        }
    }
    return ret;
}
bool DefaultWorkspace::IsExpire() {
    return false;
}

TaskInfo DefaultWorkspace::GetTaskInfo() {
    return m_task_info;
}

}



