import { BaseEntity, Column, Entity, JoinColumn, JoinTable, ManyToMany, ManyToOne, OneToMany, PrimaryColumn, PrimaryGeneratedColumn } from "typeorm";
import { Interaction } from "./interaction.entity"

@Entity()
export class Cell {
    @PrimaryColumn()
    type: string

}
