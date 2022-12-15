#ifndef INCLUDED_SAMWARRING_INSTANCE_TRACKER_HPP
#define INCLUDED_SAMWARRING_INSTANCE_TRACKER_HPP
#include <cassert>
#include <memory>
#include <samwarring/singleton.hpp>
#include <set>

namespace samwarring {

/**
 * @brief Collects statistics of instance_tracker instances.
 *
 * Objects of this structure are updated by the @ref instance_tracker class when
 * they are constructed, destroyed, assigned, and moved. See the @ref
 * instance_tracker class for more information.
 */
struct instance_tracker_stats {
    int instances{0};            /**< Number of active instances */
    int default_constructors{0}; /**< Invocation count of default constructor */
    int main_constructors{0}; /**< Invocation count of the main constructor */
    int copy_constructors{0}; /**< Invocation count of the copy constructor */
    int move_constructors{0}; /**< Invocation count of the move constructor */
    int all_constructors{0};  /**< Invocation count of all constructors */
    int destructors{0};       /**< Invocation count of all destructors */
    int copy_assignments{0};  /**< Invocation count of all copy-assignments */
    int move_assignments{0};  /**< Invocation count of all move-assignments */
    int all_assignments{0};   /**< Sum of all copy and move assignments */
    int all_copies{0};        /**< Sum of all copies */
    int all_moves{0};         /**< Sum of all moves */
    std::set<int> destroyed_ids; /**< IDs of instances that were destroyed */
    std::set<int> evicted_ids;   /**< IDs of instances that were moved-into */

  private:
    friend class instance_tracker;

    /** ID will be assigned to the next instance of instance_tracker */
    int next_id{1};
};

/**
 * @brief Tracks lifetime behavior of elements in generic containers.
 *
 * Generic containers tend to favor moves instead of copies where possible
 * to acheive greater performance. This class enables testing whether or not
 * the desired optimizations are actually taking place.
 *
 * Each object points to an @ref instance_tracker_stats structure to track
 * how many instances exist at any given time, how many copies were
 * performed, how many moves were performed, etc.
 *
 * To verify how the contents of a specific object are moved around, each
 * object is assigned a unique ID. When objects are moved-from, they are
 * assigned a "trivial" ID of 0. When a non-trivial object is destroyed, its
 * ID is added to a set of destroyed IDs in the stats structure. When a
 * non-trivial object is moved-into, its ID is added to a separate set of
 * "evicted" IDs.
 *
 * Modifications to the stats structure are not thread-safe.
 *
 * Example
 * -------
 *
 *      // Tests that emplace_back does not make temporaries.
 *      auto stats = std::make_shared<instance_tracker_stats>();
 *      std::list<instance_tracker> my_list;
 *      my_list.emplace_back(stats);
 *      my_list.emplace_back(stats);
 *      my_list.emplace_back(stats);
 *      assert(stats->instances == 3);
 *      assert(stats->all_constructors == 3);
 *      assert(stats->destructors == 0);
 *
 */
class instance_tracker {
  public:
    /**
     * @brief Construct a new default instance.
     *
     * It records its lifetime events to a reference-counted singleton stats
     * object. The singleton stats object can be pre-initialized before the
     * construction is recorded via the @ref reference_counted_singleton
     * function.
     */
    instance_tracker()
        : stats_{reference_counted_singleton<instance_tracker_stats>()} {
        assert(stats_);
        id_ = stats_->next_id++;
        stats_->instances++;
        stats_->default_constructors++;
        stats_->all_constructors++;
    }

    /**
     * @brief Constructs a new instance.
     *
     * The instance is assigned a new ID unique to the provided stats structure.
     *
     * @param stats New instance lifetime events will be tracked in this
     * object. It must not be null, or behavior is undefined.
     */
    instance_tracker(std::shared_ptr<instance_tracker_stats> stats) noexcept
        : stats_(std::move(stats)) {
        assert(stats_);
        id_ = stats_->next_id++;
        stats_->instances++;
        stats_->main_constructors++;
        stats_->all_constructors++;
    }

    /**
     * @brief Constructs a new instance via copy.
     *
     * The new instance is assigned a new ID unique to the shared stats
     * structure. It does NOT copy the ID of the other instance.
     *
     * @param other Instance to copy. Its stats structure is shared by the new
     * instance. Its ID is not modified.
     */
    instance_tracker(const instance_tracker& other) noexcept
        : stats_(other.stats_), id_{stats_->next_id++} {
        stats_->instances++;
        stats_->copy_constructors++;
        stats_->all_constructors++;
        stats_->all_copies++;
    }

    /**
     * @brief Constructs a new instance via move.
     *
     * @param other Moved-from instance. Its stats structure is shared with the
     * new instance. Its ID is moved into the new instance, then set to 0.
     */
    instance_tracker(instance_tracker&& other) noexcept
        : stats_(other.stats_), id_{other.id_} {
        other.id_ = 0;
        stats_->instances++;
        stats_->move_constructors++;
        stats_->all_constructors++;
        stats_->all_moves++;
    }

    /**
     * @brief Destroys an instance.
     *
     * The destruction is recorded in the stats structure. If this instance's ID
     * is non-trivial, the ID is added to the set of destroyed IDs.
     */
    ~instance_tracker() {
        if (id_) {
            stats_->destroyed_ids.insert(id_);
        }
        stats_->instances--;
        stats_->destructors++;
    }

    /**
     * @brief Tracks a copy-assignment between instances.
     *
     * The IDs of both instances are not modified. It is assumed both instances
     * share the same stats structure.
     *
     * @param other Instance to copy.
     */
    instance_tracker& operator=(const instance_tracker& other) noexcept {
        (void)other;
        stats_->copy_assignments++;
        stats_->all_assignments++;
        stats_->all_copies++;
        return *this;
    }

    /**
     * @brief Tracks a move-assignment between instances.
     *
     * If the current instance has a non-trivial ID, it's added to the set of
     * evicted IDs. Then the ID of the other instance is moved into the current
     * instance, setting the other to 0. It is assumed both instances share the
     * same stats structure.
     *
     * @param other Moved-from instance.
     */
    instance_tracker& operator=(instance_tracker&& other) noexcept {
        if (id_) {
            // Only track non-trivial ids that are evicted.
            stats_->evicted_ids.insert(id_);
        }
        id_ = other.id_;
        other.id_ = 0;
        stats_->move_assignments++;
        stats_->all_assignments++;
        stats_->all_moves++;
        return *this;
    }

    /**
     * @return ID of the instance.
     */
    int id() const noexcept {
        return id_;
    }

    /**
     * @return Stats structure where this instance's lifetime events are
     * recorded.
     */
    std::shared_ptr<instance_tracker_stats> stats() const noexcept {
        return stats_;
    }

  private:
    std::shared_ptr<instance_tracker_stats> stats_;
    int id_;
};

} // namespace samwarring

#endif