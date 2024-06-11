module GASRATS {
    enum Health {
        TOO_MUCH, # 0
        HIGH, # 1
        GREAT, # 2
        GOOD, # 3
        OKAY, # 4
        BAD, # 5
        TERRIBLE, # 6
        FAILING# 7
    }

    enum deployed {
        DEPLOYED
        UNDEPLOYED
    }

    enum beacon {
        INITIAL
        STANDARD
    }
}