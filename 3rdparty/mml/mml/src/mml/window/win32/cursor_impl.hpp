#ifndef MML_CURSORIMPLWIN32_HPP
#define MML_CURSORIMPLWIN32_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <mml/window/cursor.hpp>
#include <mml/system/non_copyable.hpp>

#include <windows.h>

namespace mml
{

namespace priv
{
////////////////////////////////////////////////////////////
/// \brief Win32 implementation of cursor
///
////////////////////////////////////////////////////////////
class cursor_impl : non_copyable
{
public:

    ////////////////////////////////////////////////////////////
    /// \brief Default constructor
    ///
    /// Refer to mml::cursor::cursor().
    ///
    ////////////////////////////////////////////////////////////
    cursor_impl();

    ////////////////////////////////////////////////////////////
    /// \brief Destructor
    ///
    /// Refer to mml::cursor::~cursor().
    ///
    ////////////////////////////////////////////////////////////
    ~cursor_impl();

    ////////////////////////////////////////////////////////////
    /// \brief Create a cursor with the provided image
    ///
    /// Refer to mml::cursor::load_from_pixels().
    ///
    ////////////////////////////////////////////////////////////
    bool load_from_pixels(const std::uint8_t* pixels, const std::array<std::uint32_t, 2>& size, const std::array<std::uint32_t, 2>& hotspot);

    ////////////////////////////////////////////////////////////
    /// \brief Create a native system cursor
    ///
    /// Refer to mml::cursor::load_from_system().
    ///
    ////////////////////////////////////////////////////////////
    bool load_from_system(cursor::type type);

private:

    friend class window_impl_win32;

    ////////////////////////////////////////////////////////////
    /// \brief Release the cursor, if we have loaded one.
    ///
    ////////////////////////////////////////////////////////////
    void release();

    ////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////
    HCURSOR _cursor;
};

} // namespace priv

} // namespace mml

#endif // MML_CUSROSIMPLWIN32_HPP

