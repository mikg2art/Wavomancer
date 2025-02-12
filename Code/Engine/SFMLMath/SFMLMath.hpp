//
// Created by Denis Wagner on 1/16/19.
//

#ifndef PONG_SFMLMATH_HPP
#define PONG_SFMLMATH_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>


namespace sf
{
    static const float PI = static_cast<float>(acos( -1 ));

    /*
     * Vector addition
     */
    template <typename T>
    T operator+( const T& vec1, const T& vec2 )
    {
        const auto newX = vec1.x + vec2.x;
        const auto newY = vec1.y + vec2.y;

        return T( newX, newY );
    }

    /*
     * Vector subtraction
     */
    template <typename T>
    T operator-( const T& vec1, const T& vec2 )
    {
        const auto newX = vec1.x - vec2.x;
        const auto newY = vec1.y - vec2.y;

        return T( newX, newY );
    }

    /*
     * Scalar multiplication
     */
    template <typename T, typename U>
    T operator*( const T& vec, U scalar )
    {
        const auto newX = vec.x * scalar;
        const auto newY = vec.y * scalar;

        return T( newX, newY );
    }

    /*
     * Scalar multiplication
     */
    template <typename U, typename T>
    T operator*( U scalar, const T& vec )
    {
        const auto newX = vec.x * scalar;
        const auto newY = vec.y * scalar;

        return T( newX, newY );
    }

    /*
     * Returns the dot product of two given vectors
     */
	template <typename T>
	float dot(const T& vec1, const T& vec2)
	{
		return vec1.x * vec2.x + vec1.y * vec2.y;
	}

    /*
     * Returns the square of a given value
     */
    template <typename T>
    inline T sqr( T value )
    {
        return value * value;
    }

    /*
     * Converts radians to degrees
     */
    template <typename T>
    inline float radToDeg( T radians )
    {
        return radians * 180.0f / PI;
    }

    /*
     * Converts degrees to radian
     */
    template <typename T>
    inline float degToRad( T degree )
    {
        return degree / 180.0f * PI;
    }

    /*
     * Returns the length of a given vector
     */
    template <typename T>
    inline float getLength( const T& vec )
    {
        return sqrt( sqr( vec.x ) + sqr( vec.y ) );
    }

    /*
     * Returns an inverted vector
     */
    template <typename T>
    inline T getInverted( const T& vec )
    {
        return T( -vec.x, -vec.y );
    }

    /*
     * Inverts a given vector in-place
     */
    template <typename T>
    inline T& invert( T& vec )
    {
        vec.x = -vec.x;
        vec.y = -vec.y;

        return vec;
    }

    /*
     * Returns a normalized vector
     */
    template <typename T>
    inline T getNormalized( const T& vec )
    {
        const float length = getLength( vec );
        const float newX   = vec.x / length;
        const float newY   = vec.y / length;

        return T( newX, newY );
    }

    /*
     * Normalizes a given vector in-place
     */
    template <typename T>
    inline T& normalize( T& vec )
    {
        const float length = getLength( vec );
        vec.x /= length;
        vec.y /= length;

        return vec;
    }

    /*
     * Returns the distance between two given points
     */
    template <typename T>
    float distance(const T &point1, const T &point2) {
        float dx = point2.x - point1.x;
        float dy = point2.y - point1.y;
        return std::sqrt(dx * dx + dy * dy);
    }


    /*
     * Returns the angle of a given vector from 0 to 360° depending its direction on the unit circle
     */
    template <typename T>
    inline float getRotationAngle( const T& vec )
    {
        const T      normalizedVec = getNormalized( vec );
        const float angleXRad     = acos( normalizedVec.x );
        const float angleYRad     = asin( normalizedVec.y );
        float       angleRad;

        if ( angleYRad < 0 )
            angleRad = degToRad( 360 ) - angleXRad;
        else
            angleRad = angleXRad;

        return radToDeg( angleRad );
    }

    /*
     * Returns the angle in degrees between two given vectors
     */
    template <typename T>
    inline float getAngleBetween( const T& vec1, const T& vec2 )
    {
        const float angle = acos( ( vec1 * vec2 ) / ( getLength( vec1 ) * getLength( vec2 ) ) );

        return radToDeg( angle );
    }

    /*
     * Returns a vector rotated with a given angle in degrees
     */
    template <typename T>
    inline void getRotated( const T& vec, float angle )
    {
        const float angleRad = degToRad( -angle );
        const float newX     = vec.x * cos( angleRad ) - vec.y * sin( angleRad );
        const float newY     = vec.x * sin( angleRad ) + vec.y * cos( angleRad );

        return T( newX, newY );
    }

    /*
     * Rotates a vector in-place with a given angle in degrees
     */
    template <typename T>
    inline T& rotate( T& vec, float angle )
    {
        const float angleRad = degToRad( -angle );
        
        vec.x = vec.x * cos( angleRad ) - vec.y * sin( angleRad );
        vec.y = vec.x * sin( angleRad ) + vec.y * cos( angleRad );

        return vec;
    }

    /*
     * Returns the projection of a vector on a given axis
     */
    template <typename T>
    T projection( const T& vec, const T& axis ) {
        T NullVector(0, 0);
        if (axis == NullVector) {
            return NullVector;
        }
        float k = (vec * axis) / (axis * axis);
        return k * axis;
    }


    /**
     * Returns the reflection of a vector on a given normal
     */
    template <typename T>
    T reflect(const T& vec, const T& normal) {
        return vec - 2.f * normal * (dot(normal, vec));
    }


    /**
     * Returns the normal of a vector between two points
     */
    template <typename T>
    T normalBetweenPoints(const T &point1, const T &point2) {
        T direction = point2 - point1;
        T normal = {direction.y, -direction.x};
        return sf::normalize(normal);
    }


    inline float GetRotationFromDirection(sf::Vector2f direction)
    {
        const sf::Vector2f currentPosition(1.f, 1.f);
        const sf::Vector2f targetPosition = currentPosition + direction;
        return radToDeg(std::atan2(targetPosition.x - currentPosition.x, targetPosition.y - currentPosition.y) * -1.f);
    }

    inline sf::Vector2f GetDirectionFromRotation(float angleDegrees)
    {
		const float inRadians = static_cast<float>(sf::degToRad(angleDegrees)) * -1.f;

		sf::Vector2f dir;
        dir.x = std::sin(inRadians);
        dir.y = std::cos(inRadians);

        return getNormalized(dir);
    }

    inline float GetRandomInRange(float lowerBound, float upperBound)
    {
        return lowerBound + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX / (upperBound - lowerBound)));
    }
}
#endif //PONG_SFMLMATH_HPP
