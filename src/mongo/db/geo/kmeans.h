/**
 *    Copyright (C) 2018-present MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the Server Side Public License, version 1,
 *    as published by MongoDB, Inc.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Server Side Public License for more details.
 *
 *    You should have received a copy of the Server Side Public License
 *    along with this program. If not, see
 *    <http://www.mongodb.com/licensing/server-side-public-license>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the Server Side Public License in all respects for
 *    all of the code used other than as permitted herein. If you modify file(s)
 *    with this exception, you may extend this exception to your version of the
 *    file(s), but you are not obligated to do so. If you do not wish to do so,
 *    delete this exception statement from your version. If you delete this
 *    exception statement from all source files in the program, then also delete
 *    it in the license file.
 */

#pragma once

#include <string>

#include "mongo/base/clonable_ptr.h"
#include "mongo/db/bson/dotted_path_support.h"
#include "mongo/db/geo/shapes.h"
#include "third_party/s2/s2regionunion.h"
#include "mongo/db/exec/working_set_common.h"

namespace mongo {

class KMeansClustering {
public:
    KMeansClustering();

    void ClusterFeatures(WorkingSet* const workingSet);

private:
    struct Cluster
    {
        Point centroid;
        std::vector<WorkingSetID> elements;
    };

private:
    std::map<WorkingSetID, std::unique_ptr<StoredGeometry> > mapIDtoGeom;
    std::vector<std::unique_ptr<Cluster> > clusters; 
};

}  // namespace mongo
