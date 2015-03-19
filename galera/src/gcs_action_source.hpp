//
// Copyright (C) 2010-2014 Codership Oy <info@codership.com>
//

#ifndef GALERA_GCS_ACTION_SOURCE_HPP
#define GALERA_GCS_ACTION_SOURCE_HPP

#include "action_source.hpp"
#include "galera_gcs.hpp"
#include "replicator.hpp"
#include "trx_handle.hpp"

#include "GCache.hpp"

#include "gu_atomic.hpp"

namespace galera
{
    class GcsActionSource : public galera::ActionSource
    {
    public:

        GcsActionSource(TrxHandleSlave::Pool& sp,
                        GCS_IMPL&             gcs,
                        Replicator&           replicator,
                        gcache::GCache&       gcache)
            :
            trx_pool_      (sp        ),
            gcs_           (gcs       ),
            replicator_    (replicator),
            gcache_        (gcache    ),
            received_      (0         ),
            received_bytes_(0         )
        { }

        ~GcsActionSource()
        {
            log_info << trx_pool_;
        }

        ssize_t   process(void*, bool& exit_loop);
        long long received()       const { return received_(); }
        long long received_bytes() const { return received_bytes_(); }

    private:

        void dispatch(void*, const gcs_action&, bool& exit_loop);

        TrxHandleSlave::Pool& trx_pool_;
        GCS_IMPL&             gcs_;
        Replicator&           replicator_;
        gcache::GCache&       gcache_;
        gu::Atomic<long long> received_;
        gu::Atomic<long long> received_bytes_;
    };

    class GcsActionTrx
    {
    public:
        GcsActionTrx(TrxHandleSlave::Pool& sp, const struct gcs_action& act);
        ~GcsActionTrx();
        TrxHandleSlave* trx() const { return trx_; }
    private:
        GcsActionTrx(const GcsActionTrx&);
        void operator=(const GcsActionTrx&);
        TrxHandleSlave* trx_;
    };

}

#endif // GALERA_GCS_ACTION_SOURCE_HPP
