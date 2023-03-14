#ifndef INCLUDED_SAMWARRING_QUANTITY_HPP
#define INCLUDED_SAMWARRING_QUANTITY_HPP

#include <numbers>
#include <utility>

namespace samwarring::units {

/**
 * @brief Contains a physical quantity with strongly-typed units.
 *
 * @details Objects contain only a single value of type Rep. The units of the
 * quantity are encoded in the type itself as template parameters. Each "base
 * dimension" has a template parameter for its exponent, and another for its
 * scale. Types with identical exponent parameters are said to have "compatible"
 * units, even if their scales are different. For example, meters/second and
 * miles/hour both have Distance=1 and Time=-1, but have different values for
 * DistanceScale and TimeScale.
 *
 * @tparam Rep Fundamental type used to hold the quantity value
 * @tparam Time Exponent of time dimension
 * @tparam Distance Exponent of distance dimension
 * @tparam Mass Exponent of mass dimension
 * @tparam Temperature Exponent of temperature dimension
 * @tparam Angle Exponent of angle dimension
 * @tparam TimeScale Scale compared to seconds
 * @tparam DistanceScale Scale compared to meters
 * @tparam MassScale Scale compared to grams
 * @tparam TemperatureScale Scale compared to Kelvin
 * @tparam AngleScale Scale compared to revolutions
 */
template <typename Rep = double, double Time = 0.0, double Distance = 0.0,
          double Mass = 0.0, double Temperature = 0.0, double Angle = 0.0,
          double TimeScale = 1.0, double DistanceScale = 1.0,
          double MassScale = 1.0, double TemperatureScale = 1.0,
          double AngleScale = 1.0>
class quantity {
  public:
    /**
     * @brief Convenience typedef for this expansion of the quantity template.
     */
    using self =
        quantity<Rep, Time, Distance, Mass, Temperature, Angle, TimeScale,
                 DistanceScale, MassScale, TemperatureScale, AngleScale>;

    /**
     * @brief Explicit construction from unit-less fundamental type.
     */
    explicit quantity(Rep value) : value_{value} {}

    /**
     * @brief Implicit construction from quantity with equivalent units.
     */
    template <double RhsTimeScale, double RhsDistanceScale, double RhsMassScale,
              double RhsTemperatureScale, double RhsAngleScale>
    quantity(const quantity<Rep, Time, Distance, Mass, Temperature, Angle,
                            RhsTimeScale, RhsDistanceScale, RhsMassScale,
                            RhsTemperatureScale, RhsAngleScale>& rhs) {
        value_ =
            static_cast<Rep>(rhs.value() *
                             (RhsTimeScale * RhsDistanceScale * RhsMassScale *
                              RhsTemperatureScale * RhsAngleScale) /
                             (TimeScale * DistanceScale * MassScale *
                              TemperatureScale * AngleScale));
    }

    /**
     * @brief Assignment from quantity with equivalent units.
     */
    template <double RhsTimeScale, double RhsDistanceScale, double RhsMassScale,
              double RhsTemperatureScale, double RhsAngleScale>
    self&
    operator=(const quantity<Rep, Time, Distance, Mass, Temperature, Angle,
                             RhsTimeScale, RhsDistanceScale, RhsMassScale,
                             RhsTemperatureScale, RhsAngleScale>& rhs) {
        value_ =
            static_cast<Rep>(rhs.value() *
                             (RhsTimeScale * RhsDistanceScale * RhsMassScale *
                              RhsTemperatureScale * RhsAngleScale) /
                             (TimeScale * DistanceScale * MassScale *
                              TemperatureScale * AngleScale));
    }

    /**
     * @brief Extracts the underlying un-scaled value.
     *
     * @example auto s = seconds{20}.value(); // s = 20;
     *          auto h = hours{20}.value();   // h = 20;
     */
    Rep value() const {
        return value_;
    }

  private:
    Rep value_;
};

/**
 * @brief Compares equivalence between two quantities.
 *
 * @details The template is only defined for quanties with the same exponent
 * values. Their scales however, may be different.
 *
 * @return true if lhs and rhs represent the same quantity.
 *
 * @example bool b = (seconds{120} == minutes{2}); // b = true;
 */
template <class Rep, double Time, double Distance, double Mass,
          double Temperature, double Angle, double LhsTimeScale,
          double RhsTimeScale, double LhsDistanceScale, double RhsDistanceScale,
          double LhsMassScale, double RhsMassScale, double LhsTemperatureScale,
          double RhsTemperatureScale, double LhsAngleScale,
          double RhsAngleScale>
bool operator==(const quantity<Rep, Time, Distance, Mass, Temperature, Angle,
                               LhsTimeScale, LhsDistanceScale, LhsMassScale,
                               LhsTemperatureScale, LhsAngleScale>& lhs,

                const quantity<Rep, Time, Distance, Mass, Temperature, Angle,
                               RhsTimeScale, RhsDistanceScale, RhsMassScale,
                               RhsTemperatureScale, RhsAngleScale>& rhs) {

    static constexpr double lhs_scale = LhsTimeScale * LhsDistanceScale *
                                        LhsMassScale * LhsTemperatureScale *
                                        LhsAngleScale;

    static constexpr double rhs_scale = RhsTimeScale * RhsDistanceScale *
                                        RhsMassScale * RhsTemperatureScale *
                                        RhsAngleScale;

    if constexpr (lhs_scale == rhs_scale) {
        return lhs.value() == rhs.value();
    } else {
        return (lhs.value() * lhs_scale) == (rhs.value() * rhs_scale);
    }
}

/**
 * @brief Multiplies the quantity by a constant unit-less value.
 *
 * @tparam Rep Representation of input quantity.
 * @tparam MulRep Data type of the constant value.
 *
 * @return A quantity with equivalent units and scale. The representation of the
 * output quantity is defined as the type returned from multiplying objects of
 * type Rep and MulRep.
 */
template <typename Rep, typename MulRep, double Time, double Distance,
          double Mass, double Temperature, double Angle, double TimeScale,
          double DistanceScale, double MassScale, double TemperatureScale,
          double AngleScale>
quantity<decltype(std::declval<Rep>() * std::declval<MulRep>()), Time, Distance,
         Mass, Temperature, Angle, TimeScale, DistanceScale, MassScale,
         TemperatureScale, AngleScale>
operator*(quantity<Rep, Time, Distance, Mass, Temperature, Angle, TimeScale,
                   DistanceScale, MassScale, TemperatureScale, AngleScale>
              lhs,

          MulRep rhs) {

    return quantity<decltype(std::declval<Rep>() * std::declval<MulRep>()),
                    Time, Distance, Mass, Temperature, Angle, TimeScale,
                    DistanceScale, MassScale, TemperatureScale, AngleScale>{
        lhs.value() * rhs};
}

/**
 * @brief Multiplies two quantities.
 *
 * @return A quantity whose value is the product of the 2 inputs' values. The
 * exponents of the output are the pairwise products of the inputs' exponents.
 * The scales of the output are the pairwase products of the inputs' scales. The
 * representation of the output quantity is defined as the type obtained by
 * multiplying objects of the inputs' representations.
 */
template <typename LhsRep, typename RhsRep, double LhsTime, double RhsTime,
          double LhsDistance, double RhsDistance, double LhsMass,
          double RhsMass, double LhsTemperature, double RhsTemperature,
          double LhsAngle, double RhsAngle, double LhsTimeScale,
          double RhsTimeScale, double LhsDistanceScale, double RhsDistanceScale,
          double LhsMassScale, double RhsMassScale, double LhsTemperatureScale,
          double RhsTemperatureScale, double LhsAngleScale,
          double RhsAngleScale>

quantity<decltype(std::declval<LhsRep>() * std::declval<RhsRep>()),
         (LhsTime * RhsTime), (LhsDistance * RhsDistance), (LhsMass * RhsMass),
         (LhsTemperature * RhsTemperature), (LhsAngle * RhsAngle),
         (LhsTimeScale * RhsTimeScale), (LhsDistanceScale * RhsDistanceScale),
         (LhsMassScale * RhsMassScale),
         (LhsTemperatureScale * RhsTemperatureScale),
         (LhsAngleScale * RhsAngleScale)>

operator*(quantity<LhsRep, LhsTime, LhsDistance, LhsMass, LhsTemperature,
                   LhsAngle, LhsTimeScale, LhsDistanceScale, LhsMassScale,
                   LhsTemperatureScale, LhsAngleScale>
              lhs,

          quantity<RhsRep, RhsTime, RhsDistance, RhsMass, RhsTemperature,
                   RhsAngle, RhsTimeScale, RhsDistanceScale, RhsMassScale,
                   RhsTemperatureScale, RhsAngleScale>
              rhs) {

    return quantity<decltype(std::declval<LhsRep>() * std::declval<RhsRep>()),
                    (LhsTime * RhsTime), (LhsDistance * RhsDistance),
                    (LhsMass * RhsMass), (LhsTemperature * RhsTemperature),
                    (LhsAngle * RhsAngle), (LhsTimeScale * RhsTimeScale),
                    (LhsDistanceScale * RhsDistanceScale),
                    (LhsMassScale * RhsMassScale),
                    (LhsTemperatureScale * RhsTemperatureScale),
                    (LhsAngleScale * RhsAngleScale)>{lhs.value() * rhs.value()};
}

template <class LhsRep, class RhsRep, double LhsTime, double RhsTime,
          double LhsDistance, double RhsDistance, double LhsMass,
          double RhsMass, double LhsTemperature, double RhsTemperature,
          double LhsAngle, double RhsAngle, double LhsTimeScale,
          double RhsTimeScale, double LhsDistanceScale, double RhsDistanceScale,
          double LhsMassScale, double RhsMassScale, double LhsTemperatureScale,
          double RhsTemperatureScale, double LhsAngleScale,
          double RhsAngleScale>

quantity<decltype(std::declval<LhsRep>() / std::declval<RhsRep>()),
         (LhsTime - RhsTime), (LhsDistance - RhsDistance), (LhsMass - RhsMass),
         (LhsTemperature - RhsTemperature), (LhsAngle - RhsAngle),
         (LhsTimeScale / RhsTimeScale), (LhsDistanceScale / RhsDistanceScale),
         (LhsMassScale / RhsMassScale),
         (LhsTemperatureScale / RhsTemperatureScale),
         (LhsAngleScale / RhsAngleScale)>

operator/(const quantity<LhsRep, LhsTime, LhsDistance, LhsMass, LhsTemperature,
                         LhsAngle, LhsTimeScale, LhsDistanceScale, LhsMassScale,
                         LhsTemperatureScale, LhsAngleScale>& lhs,

          const quantity<RhsRep, RhsTime, RhsDistance, RhsMass, RhsTemperature,
                         RhsAngle, RhsTimeScale, RhsDistanceScale, RhsMassScale,
                         RhsTemperatureScale, RhsAngleScale>& rhs) {

    return quantity<decltype(std::declval<LhsRep>() * std::declval<RhsRep>()),
                    (LhsTime / RhsTime), (LhsDistance / RhsDistance),
                    (LhsMass / RhsMass), (LhsTemperature / RhsTemperature),
                    (LhsAngle / RhsAngle), (LhsTimeScale / RhsTimeScale),
                    (LhsDistanceScale / RhsDistanceScale),
                    (LhsMassScale / RhsMassScale),
                    (LhsTemperatureScale / RhsTemperatureScale),
                    (LhsAngleScale / RhsAngleScale)>{lhs.value() / rhs.value()};
}

// -----------------------------------------------------------------------------
//                             Pre-defined Units
// -----------------------------------------------------------------------------
#ifdef DEFINE_UNIT
#error The DEFINE_UNIT macro is already defined
#endif
#define DEFINE_UNIT(UnitName, Time, Distance, Mass, Temperature, Angle,        \
                    TimeScale, DistanceScale, MassScale, TemperatureScale,     \
                    AngleScale)                                                \
    template <typename Rep>                                                    \
    class UnitName                                                             \
        : public quantity<Rep, Time, Distance, Mass, Temperature, Angle,       \
                          TimeScale, DistanceScale, MassScale,                 \
                          TemperatureScale, AngleScale> {                      \
      public:                                                                  \
        using quantity<Rep, Time, Distance, Mass, Temperature, Angle,          \
                       TimeScale, DistanceScale, MassScale, TemperatureScale,  \
                       AngleScale>::quantity;                                  \
    };

// clang-format off
//          Name     Time     Dist       Mass     Temp    Ang
DEFINE_UNIT(seconds,  1.0,     0.0,       0.0,     0.0,   0.0,
                      1.0,     1.0,       1.0,     1.0,   1.0
           );
DEFINE_UNIT(minutes,   1.0,    0.0,       0.0,     0.0,   0.0,
                      60.0,    1.0,       1.0,     1.0,   1.0
           );
DEFINE_UNIT(hours,     1.0,    0.0,       0.0,     0.0,   0.0,
                    3600.0,    1.0,       1.0,     1.0,   1.0
           );
DEFINE_UNIT(meters,    0.0,    1.0,       0.0,     0.0,   0.0,
                       1.0,    1.0,       1.0,     1.0,   1.0
           );
DEFINE_UNIT(grams,     0.0,    0.0,       1.0,     0.0,   0.0,
                       1.0,    1.0,       1.0,     1.0,   1.0
           );
DEFINE_UNIT(kilograms, 0.0,    0.0,       1.0,     0.0,   0.0,
                       1.0,    1.0,    1000.0,     1.0,   1.0
           );
DEFINE_UNIT(kelvin,    0.0,    0.0,       0.0,     1.0,   0.0,
                       1.0,    1.0,       1.0,     1.0,   1.0
           );
DEFINE_UNIT(rankine,   0.0,    0.0,       0.0,     1.0,   0.0,
                       1.0,    1.0,       1.0,     1.8,   1.0
           );
DEFINE_UNIT(revolutions, 0.0,  0.0,       0.0,     0.0,   1.0,
                       1.0,    1.0,       1.0,     1.0,   1.0
           );
DEFINE_UNIT(radians,   0.0,    0.0,       0.0,     0.0,   1.0,
                       1.0,    1.0,       1.0,     1.0,   (1.0/(2*std::numbers::pi))
           );
DEFINE_UNIT(degrees,   0.0,    0.0,       0.0,     0.0,   1.0,
                       1.0,    1.0,       1.0,     1.0,   (1.0/360.0)
           );
// clang-format on

#undef DEFINE_UNIT

} // namespace samwarring::units

#endif