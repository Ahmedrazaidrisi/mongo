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


#include "mongo/db/geo/geometry_container.h"
#include "mongo/db/geo/kmeans.h"

#include "mongo/db/geo/geoconstants.h"
#include "mongo/db/geo/geoparser.h"
#include "mongo/logv2/log.h"
#include "mongo/util/str.h"
#include "mongo/util/transitional_tools_do_not_use/vector_spooling.h"

#define MONGO_LOGV2_DEFAULT_COMPONENT ::mongo::logv2::LogComponent::kQuery

// Hardcode these values for now
#define MAX_CLUSTERING_ITERATIONS 5
#define NUM_CLUSTERS 5

namespace mongo {

KMeansClustering::KMeansClustering()
{
}

void KMeansClustering::ClusterFeatures(WorkingSet* const workingSet, const string& field)
{
    // Read all geometries from working set into mapIDtoGeom
    std::vector<std::unique_ptr<StoredGeometry>> geometries;
    for (WorkingSetID i = 0; i < workingSet->getSize(); i++)
    {
        WorkingSetMember* wsm = workingSet->get(i);
        StoredGeometry::extractGeometries(wsm->doc.value().toBson(), field, &geometries, true);
        geoms.push_back(std::move(geometries[0]));
        geometries.clear();
        workingSet->free(i);
    }

    // Create empty clisters with random centroinds
    std::srand(std::time(nullptr)); // use current time as seed for random generator
    clusters.resize(NUM_CLUSTERS);
    for (int i = 0; i < NUM_CLUSTERS; i++)
    {
        std::unique_ptr<Cluster> cluster(new Cluster());
                
        // Divide the range of elements in geoms array into equal NUM_CLUSTERS parts
        // and get random element from each of the part for the corresponding cluster. 
        int centroidIdx = (std::rand() % (geoms.size() / NUM_CLUSTERS)) + (geoms.size() / NUM_CLUSTERS) * i;
        cluster->centroid = geoms[centroidIdx]->geometry.getPoint().oldPoint;
        clusters[i] = std::move(cluster);
    }

    // Cluster features
    for (int iteration = 0; iteration < MAX_CLUSTERING_ITERATIONS; iteration++)
    {
        // Clear clusters content
        // Reassing geometries to clusters based on the distance to centroids
        // Recalculate centroids
        // Break if centroids don't change
    }

    // Add/Update clusterID field in the working set
    /*for (int i = 0; i < clusters.size(); i++)
    {
        for (int e = 0; e < clusters[i]->elements.size(); e++)
        {
            // Get clusters[i]->elements[e] from workingSet 
            // Store clusterID = e into that element
        }
    }*/
}

}  // namespace mongo
