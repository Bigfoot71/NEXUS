#ifndef NEXUS_UTILS_CONTEXTUAL_HPP
#define NEXUS_UTILS_CONTEXTUAL_HPP

#include "../platform/nxPlatform.hpp"

#include <utility>
#include <memory>

namespace nexus { namespace utils {

    /**
     * @brief Template struct for managing context-specific data.
     * 
     * The Contextual struct is a template allowing for the management of context-specific data, 
     * such as data sent to the GPU. It is permanently associated with a Context type and serves 
     * as the parent structure for final contextual objects, such as Texture, Shader, Mesh, etc.
     * 
     * In summary, each class requiring a specific context to function will use a container that 
     * stores this struct and the derived data in the form of a std::shared_ptr. However, the 
     * extensive copying of such resources may require additional resources due to the 
     * thread-safe nature of std::shared_ptr's reference counter. It is therefore recommended to 
     * prefer transferring contextual data by reference or by move, unless copying is absolutely necessary.
     * 
     * @tparam T_Context The type of context associated with the contextual data.
     */
    template <typename T_Context>
    struct NEXUS_API Contextual
    {
        using Context = T_Context;
        Context &ctx;               ///< Reference to the associated context.

        /**
         * @brief Constructs a Contextual object with the specified context.
         * @param ctx The context associated with the contextual data.
         */
        Contextual(Context& ctx) : ctx(ctx) { }

        /**
         * @brief Virtual destructor for Contextual.
         * 
         * Ensures proper destruction of derived classes.
         */
        virtual ~Contextual() = default;

        Contextual(const Contextual&) = delete; ///< Deleted copy constructor.
        Contextual& operator=(const Contextual&) = delete; ///< Deleted copy assignment operator.
    };

    /**
     * @brief Template struct to check if a class is derived from Contextual.
     * 
     * The IsContextual struct is a template used to determine if a given class 
     * is derived from the Contextual template struct. It evaluates to true if the 
     * class is derived from Contextual, otherwise it evaluates to false.
     * 
     * @tparam Derived The class to check.
     */
    template <typename Derived>
    struct NEXUS_API IsContextual
    {
        /**
        * @brief Boolean value indicating if Derived is derived from Contextual.
        */
        static constexpr bool value = std::is_base_of<Contextual<typename Derived::Context>, Derived>::value;
    };

    /**
     * @brief Template class serving as the parent class for contextual resources.
     * 
     * The Container class is a template class that acts as the parent class for various
     * contextual resources. It contains a shared pointer to the contextual resource.
     * 
     * @tparam T_Contextual The type of contextual resource.
     */
    template <typename T_Contextual>
    class NEXUS_API Container
    {
        static_assert(IsContextual<T_Contextual>::value,
            "[Container] <T_Contextual> must be derived from Contextual");

      public:
        using Type = T_Contextual;                          ///< Type alias for the contextual resource type.
        using Context = typename T_Contextual::Context;     ///< Type alias for the associated context type.

      protected:
        std::shared_ptr<T_Contextual> obj;                  ///< Shared pointer to the contextual resource.

      public:
        /**
         * @brief Constructs a new contextual object that can be shared.
         * 
         * This constructor creates a new contextual object with the specified context and arguments.
         * 
         * @tparam Args Parameter pack for forwarding arguments to the contextual object constructor.
         * @param ctx The context associated with the contextual object.
         * @param args Arguments forwarded to the constructor of the contextual object.
         */
        template <typename... Args>
        Container(typename T_Contextual::Context& ctx, Args&&... args)
        {
            using Context = typename std::remove_pointer<typename std::remove_reference<decltype(ctx)>::type>::type;
            obj = std::make_shared<T_Contextual>(ctx, std::forward<Args>(args)...);
        }

        /**
         * @brief Destructor for Container.
         * 
         * The destructor ensures proper cleanup of the contextual object.
         */
        virtual ~Container() = default;

        /**
         * @brief Explicit copy constructor.
         * 
         * This constructor creates a new reference to the shared data.
         * 
         * @param other The Container object to copy from.
         */
        Container(const Container& other)
            : obj(other.obj)
        { }

        /**
         * @brief Explicit move constructor.
         * 
         * This constructor moves the pointer to the shared data from another Container object.
         * 
         * @param other The Container object to move from.
         */
        Container(Container&& other) noexcept
            : obj(std::move(other.obj))
        { }

        /**
         * @brief Explicit copy assignment operator.
         * 
         * This operator assigns the shared data from another Container object.
         * 
         * @param other The Container object to copy from.
         * @return A reference to the modified Container object.
         */
        Container& operator=(const Container& other)
        {
            if (this != &other)
            {
                obj = other.obj;
            }
            return *this;
        }

        /**
         * @brief Explicit move assignment operator.
         * 
         * This operator moves the shared data from another Container object.
         * 
         * @param other The Container object to move from.
         * @return A reference to the modified Container object.
         */
        Container& operator=(Container&& other) noexcept
        {
            if (this != &other)
            {
                obj = std::move(other.obj);
            }
            return *this;
        }

        /**
         * @brief Allows access to the contained contextual object.
         * 
         * This operator allows access to the contained contextual object 
         * via the -> operator.
         * 
         * @return A pointer to the contained contextual object.
         */
        T_Contextual* operator->() { return obj.get(); }

        /**
         * @brief Allows access to the contained contextual object (const version).
         * 
         * This operator allows access to the contained contextual object 
         * via the -> operator (const version).
         * 
         * @return A const pointer to the contained contextual object.
         */
        const T_Contextual* operator->() const { return obj.get(); }

        /**
         * @brief Equality comparison operator.
         * 
         * This operator compares the shared data of two Container objects for equality.
         * 
         * @param other The Container object to compare against.
         * @return True if the shared data of both Container objects are equal, false otherwise.
         */
        bool operator==(const Container& other) const { return obj == other.obj; }

        /**
         * @brief Inequality comparison operator.
         * 
         * This operator compares the shared data of two Container objects for inequality.
         * 
         * @param other The Container object to compare against.
         * @return True if the shared data of both Container objects are not equal, false otherwise.
         */
        bool operator!=(const Container& other) const { return obj != other.obj; }

        /**
         * @brief Gets a pointer to the context of the contained object.
         * 
         * This function returns a const reference to the context associated with the contained object.
         * 
         * @return A const reference to the context associated with the contained object.
         */
        const typename T_Contextual::Context& GetContext() const { return obj->ctx; }

        /**
         * @brief Gets a pointer to the context of the contained object.
         * 
         * This function returns a reference to the context associated with the contained object.
         * 
         * @return A reference to the context associated with the contained object.
         */
        typename T_Contextual::Context& GetContext() { return obj->ctx; }

        /**
         * @brief Checks if the given context is the same as the one associated with the object.
         * 
         * This function compares the given context with the context associated with the object.
         * 
         * @param ctx The context to compare.
         * @return True if the given context is the same as the one associated with the object, false otherwise.
         */
        bool HasSameContext(const typename T_Contextual::Context& ctx) const { return &obj->ctx == &ctx; }

        /**
         * @brief Checks if the context of the other contextual object is the same as the one associated with the object.
         * 
         * This function compares the context of another contextual object with the context associated with the object.
         * 
         * @param other The other contextual object to compare.
         * @return True if the context of the other contextual object is the same as the one associated with the object, false otherwise.
         */
        bool HasSameContext(const Container& other) const { return obj->ctx == &other.obj->ctx; }

        /**
         * @brief Gets the number of existing references to the underlying object.
         * 
         * This function returns the number of existing references to the underlying object.
         * 
         * @return The number of existing references to the underlying object.
         */
        auto UseCount() const { return obj.use_count(); }
    };

    /**
     * @brief Template struct to check if a class is derived from Container.
     * 
     * The IsContainer struct is a template used to determine if a given class is derived 
     * from the Container template class. It evaluates to true if the class is derived from 
     * Container, otherwise it evaluates to false.
     * 
     * @tparam Derived The class to check.
     */
    template <typename Derived>
    struct NEXUS_API IsContainer
    {
        /**
         * @brief Helper function to check if the class is derived from Container.
         * 
         * This function checks if the class is derived from Container by using 
         * std::is_base_of type trait.
         * 
         * @tparam T The type to check against.
         * @param pointer Pointer to an object of type T.
         * @return True if Derived is derived from Container<T>, false otherwise.
         */
        template <typename T>
        static constexpr bool check(const Container<T>*)
        {
            return std::is_base_of<Container<T>, Derived>::value;
        }

        /**
         * @brief Boolean value indicating if Derived is derived from Container.
         */
        static constexpr bool value = check(static_cast<Derived*>(nullptr));
    };

}}

#endif //NEXUS_UTILS_CONTEXTUAL_HPP
