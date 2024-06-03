@Module containging ports and types specific to the flight logic
module FL {
    @ Sends a signal to another component
    port ping ()
    @ Collects voltage and current data from the EPS
    port epsHealth (ref voltage: F32,ref current: F32) -> bool
    @ Pings a componenet to return a U32
    port getClock () -> U32
    @ Pings another component and returns a bool
    port deploy () -> bool
    @ Pings another component and returns a bool
    port getHealth () -> GASRATS.Health
}