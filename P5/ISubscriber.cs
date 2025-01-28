// Jomikael Ruiz
// CPSC 3200
// ISubscriber.cs
// Date Created: 11/25/24
// Last Modified: 12/2/24
namespace P5;

/**
 * Interface Invariant:
 * - ISubscriber represents the idea of a subscriber object, enforcing
 *   messaging properties across all objects that inherit ISubscriber
*/
public interface ISubscriber
{
    void NewMessage(string message);
}
/*
 * Implementation Invariant:
 * - Classes that inherit ISubscriber must implement NewMessage()
 */